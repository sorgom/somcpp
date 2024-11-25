#include <SOM/docopts.h>
#include <SOM/BaseLib.h>
#include <SOM/pyregex.h>

#include <iostream>
#include <regex>
#include <set>
#include <string>
using
    py::repl,
    std::regex, std::string, std::cout, std::cerr;

bool DocOpts::process(const INT32 argc, const CONST_C_STRING* const argv, const INT32 start)
{
    reset();

    mOk = true;
    static const regex reHelp("(?:^|\n) *-([a-zA-Z])( +<\\w+[\\w ]*>)?");
    static const regex reOpt("^-([a-zA-Z]*)$");

    std::set<CHAR>keys;

    CONST_C_STRING ch = mHelp;
    std::cmatch cm;

    while (mOk and std::regex_search(ch, cm, reHelp))
    {
        CHAR key = cm[1].str()[0];
        if (keys.find(key) != keys.end())
        {
            cerr << "duplicate option definition: -" << key << '\n';
            mOk = false;
        }
        keys.insert(key);
        if (cm[2].matched) mValueKeys.insert(key);
        else mSwitchKeys.insert(key);
        ch = cm.suffix().first;
    }
    if (mOk and argc > start)
    {
        std::set<CHAR> doneValues;
        INT32 n = start;
        while (mOk and n < argc)
        {
            if (std::regex_match(argv[n], cm, reOpt))
            {
                for (auto c : cm[1].str())
                {
                    if (keys.find(c) == keys.end())
                    {
                        cerr << "unknown option: -" << c << '\n';
                        mOk = false;
                    }
                    else if (mValueKeys.find(c) != mValueKeys.end())
                    {
                        if (doneValues.find(c) != doneValues.end())
                        {
                            cerr << "duplicate option usage: -" << c << '\n';
                            mOk = false;
                        }
                        else if (n < argc - 1)
                        {
                            doneValues.insert(c);
                            mValues[c] = argv[n + 1];
                            ++n;
                        }
                        else
                        {
                            cerr << "no value for option: -" << c << '\n';
                            mOk = false;
                        }
                    }
                    else mSwitches.insert(c);
                }
                ++n;
            }
            else
            {
                mArgs = argv + n;
                mArgc = argc - n;
                break;
            }
        }
    }
    if (not mOk) reset();
    return mOk;
}

void DocOpts::toShell() const
{
    if (mOk)
    {
    #ifdef _WIN32
        static const CONST_C_STRING prefix = "set _";
        static const CONST_C_STRING argsQuote = "";
        static const CONST_C_STRING cTrue  = "1==1";
        static const CONST_C_STRING cFalse = "0==1";

        for (const auto c : mSwitchKeys)
        {
            cout << prefix << c;
            //  win cmd is not case sensitive
            //  so:
            // -x will set %_x%
            // -X will set %_Xu%
            if (std::isupper(c)) cout << 'u';
            cout  << '=' << (isSet(c) ? cTrue : cFalse ) << '\n';
        }

    #else
        static const CONST_C_STRING prefix = "export _";
        static const CONST_C_STRING argsQuote = "\"";
        static const CONST_C_STRING cTrue  = "true";
        static const CONST_C_STRING cFalse = "false";
        for (const auto c : mSwitchKeys)
            cout << prefix << c << '=' << (isSet(c) ? cTrue : cFalse ) << '\n';
    #endif

        CONST_C_STRING value;
        for (const auto c : mValueKeys)
            cout << prefix << c << '=' << (getValue(value, c) ? value : "") << '\n';

        cout << prefix << "args=" << argsQuote;
        for (INT32 i = 0; i < mArgc; ++i) cout << mArgs[i] << ' ';
        cout << argsQuote << '\n';
    }
}

void DocOpts::help(CONST_C_STRING argv0) const
{
    static const regex reHelp("ARGV0");

    cout << repl(reHelp, baseName(argv0), mHelp) << '\n';
}

void DocOpts::reset()
{
    mArgs = nullptr;
    mArgc = 0;
    mValues.clear();
    mSwitches.clear();
    mValueKeys.clear();
    mSwitchKeys.clear();
    mOk = false;
}

bool DocOpts::getValue(CONST_C_STRING& value, CHAR key) const
{
    const auto it = mValues.find(key);
    const bool ok = it != mValues.end();
    if (ok)
    {
        value = it->second;
    }
    return ok;
}
