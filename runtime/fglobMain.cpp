#include <SOM/fglob.h>
#include <iostream>
using std::cout;

class GlobOut : public I_GlobProcessor
{
public:
    inline GlobOut()
    {
#ifdef _WIN32
        cout << "set _args=";
#else
        cout << "export _args=\"";
#endif
    }
    inline ~GlobOut()
    {
#ifndef _WIN32
        cout << '"';
#endif
        cout << '\n';
    }

    void process(CONST_C_STRING item) override
    {
        cout << item << ' ';
    }
};

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    GlobOut proc;
    for (INT32 i = 1; i < argc; ++i)
    {
        fglob(argv[i], proc);
    }
    return 0;
}
