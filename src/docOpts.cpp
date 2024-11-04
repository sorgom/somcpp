#include <SOM/docOpts.h>
#include <regex>
#include <string>
#include <set>
#include <iostream>
using std::regex, std::string, std::cout;

bool DocOpts::process(const CONST_C_STRING help, const INT32 argc, const CONST_C_STRING* const argv)
{
    rmArgs();
    mValues.clear();
    mSwitches.clear();
    std::smatch res;
    static const regex reHelp("(?:^|\n) *-([a-zA-Z])( +<\\w+)?");
    static const regex reOpt("^-[a-zA-Z]*$");
    const string s(help);

    std::set<CHAR>vKeys;
    std::set<CHAR>keys;
    string myOpts("^-([");

    string::const_iterator searchStart( s.cbegin() );
    while (std::regex_search(searchStart, s.cend(), res, reHelp) )
    {
        cout << res.size() << ' ' << res[1] << " >" << res[2] << '<'<<'\n';
        CHAR key = res[1].str()[0];
        myOpts += key;
        keys.insert(key);
        if (res[2].matched)
        {
            vKeys.insert(key);
        }  
        searchStart = res.suffix().first;
    }
    for (auto key : vKeys)
    {
        cout << "key: " << key << '\n';
    }
    myOpts += "]+)$";
    cout << "myOpts: " << myOpts << '\n';
    mArgs = new CONST_C_STRING[argc];

    mArgc = 0;
    std::cmatch m;
    const regex reArg(myOpts);
    cout << "argc: " << argc << '\n';
    for (INT32 n = 1; n < argc; ++n)
    {
        cout << "argv[" << n << "]: " << argv[n] << '\n';   
        if (std::regex_match(argv[n], m, reArg))
        {
            for (auto c : m.str())
            {
                if (vKeys.find(c) != vKeys.end())
                {
                    cout << "vKey: " << c << '\n';
                    if (n < argc - 1)
                    {
                        mValues[c] = argv[n + 1];
                        ++n;
                    }
                    else
                    {
                        cout << "missing value for key: " << c << '\n';
                        return false;
                    }
                }
                else
                {
                    mSwitches.insert(c);
                }
            }
        }
        else
        {
            mArgs[mArgc++] = argv[n];
        }
    }
    return true;
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
    }
}