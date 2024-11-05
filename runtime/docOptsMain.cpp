#include <SOM/docOpts.h>
#include <SOM/fio.h>

#include <filesystem>
#include <fstream>

#include <iostream>
using std::cout;
#include <ciso646>


INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    INT32 ret = 1;
    DocOpts opts;
    std::string help;
    if (
        argc > 2 and
        read(help, argv[1], false) and
        opts.process(help.c_str(), argc, argv, 2)
    )
    {
        opts.toShell();
        ret = 0;
    }
    return ret;
}
