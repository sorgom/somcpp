#include <SOM/BaseTypes.h>
#include <SOM/TraceMacros.h>

#include <SOM/packBegin.h>
#include <SOM/packEnd.h>

#include <SOM/docOpts.h>

#include <iostream>
using std::cout;
#include <ciso646>

const CONST_C_STRING help =
"\nconvert covbr text files to html\n\n"
"usage: " __FILE__ " [options] covbr files\n"
"options:\n"
"-c  print cmd text\n"
"-w  write back cleaned covbr text files\n"
"-t  <target> target directory\n"
"-h  this help\n";

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC_TIME()
    DocOpts opts;
    if (not opts.process(help, argc, argv))
    {
        return 1;
    }
    CONST_C_STRING ret;
    if (opts.isSet('h'))
    {
        cout << help;
        return 0;
    }
    for (const auto c : opts.activeSwitches())
    {
        switch(c)
        {
            case 'c':
                opts.toCmd();
                return 0;
            default:
                break;
        }
    }
    if (opts.getValue(ret, 't'))
    {
        cout << "target: " << ret << '\n';
    }
    cout << "argc: " << opts.argc() << '\n';
    for (INT32 i = 0; i < opts.argc(); ++i)
    {
        cout << "arg[" << i << "]: " << opts.args()[i] << '\n';
    }
    return 0;
}
