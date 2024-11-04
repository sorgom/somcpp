#include <SOM/docOpts.h>
#include <regex>
#include <string>
#include <set>
#include <iostream>
#include <ciso646>
using std::regex, std::string, std::cout, std::cerr;

bool DocOpts::process(const CONST_C_STRING help, const INT32 argc, const CONST_C_STRING* const argv, const INT32 start)
{
    reset();

    if (argc > start)
    {
        mOk = true;
        static const regex reHelp("(?:^|\n) *-([a-zA-Z])( +<\\w+)?");
        static const regex reOpt("^-([a-zA-Z]*)$");

        std::set<CHAR>keys;

        CONST_C_STRING ch = help;
        std::cmatch cm;
        while (std::regex_search(ch, cm, reHelp) )
        {
            // cout << res.size() << ' ' << res[1] << " >" << res[2] << '<'<<'\n';
            CHAR key = cm[1].str()[0];
            keys.insert(key);
            if (cm[2].matched) mValueKeys.insert(key);
            else mSwitchKeys.insert(key);
            ch = cm.suffix().first;
        }
        mArgs = new CONST_C_STRING[argc - 1];
        mArgc = 0;
        std::set<CHAR> doneValues;
        for (INT32 n = start; mOk and n < argc; ++n)
        {
            // cout << "argv[" << n << "]: " << argv[n] << '\n';   
            if (std::regex_match(argv[n], cm, reOpt))
            {
                for (auto c : cm[1].str())
                {
                    // cout << "c: " << c << '\n';
                    if (keys.find(c) == keys.end())
                    {
                        cerr << "unknown option: -" << c << '\n';
                        mOk = false;
                    }
                    else if (mValueKeys.find(c) != mValueKeys.end())
                    {
                        // cout << "vKey: " << c << '\n';
                        if (doneValues.find(c) != doneValues.end())
                        {
                            cerr << "duplicate option: -" << c << '\n';
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
            }
            else mArgs[mArgc++] = argv[n];
        }
        if (not mOk) reset();
    }
    return mOk;
}

void DocOpts::toCmd() const
{
    if (mOk)
    {
        cout << "set _opts=";
        for (const auto c : mSwitchKeys)
        {
            cout << '_' << c << ' ';
        }
        for (const auto c : mValueKeys)
        {
            cout << '_' << c << ' ';
        }
        cout << '\n';

        for (const auto c : mSwitchKeys)
        {
            cout << "set _" << c << '=' << (isSet(c) ? '1' : '0' ) << '\n';
        }
        for (const auto c : mValueKeys)
        {
            CONST_C_STRING value;
            cout << "set _" << c << '=' << (getValue(value, c) ? value : "") << '\n';
        }
        cout << "set _args=";
        for (INT32 i = 0; i < mArgc; ++i)
        {
            cout << mArgs[i] << ' ';
        }
        cout << '\n';
    }    
}

void DocOpts::reset()
{
    rmArgs();
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

void DocOpts::rmArgs()
{
    if (mArgs != nullptr)
    {
        delete[] mArgs;
        mArgs = nullptr;
    }
    mArgc = 0;
}