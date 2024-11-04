#include <SOM/BaseTypes.h>
#include <SOM/TraceMacros.h>

#include <SOM/packBegin.h>
#include <SOM/packEnd.h>

#include <SOM/docOpts.h>

#include <iostream>
using std::cout;

const CONST_C_STRING help = 
"\nconvert covbr text files to html\n\n"
"usage: " __FILE__ " [options] covbr files\n"
"options:\n"
"-c  highlight covered items\n"
"-w  write back cleaned covbr text files\n"
"-t  <target> target directory\n"
"-h  this help\n";

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC_TIME()
    DocOpts opts;
    opts.process(help, argc, argv);
    CONST_C_STRING ret;
    if (opts.getValue(ret, 't'))
    {
        cout << "target: " << ret << '\n';
        return 0;
    }
}

