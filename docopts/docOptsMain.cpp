#include <SOM/BaseTypes.h>
#include <SOM/TraceMacros.h>

#include <SOM/docOpts.h>
#include <filesystem>
#include <fstream>

#include <iostream>
using std::cout;
#include <ciso646>


INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TRACE_FUNC_TIME()
    TRACE_VAR(argc)
    INT32 ret = 1;
    if (argc > 2 and std::filesystem::is_regular_file(argv[1]))
    {
        std::ifstream is(argv[1]);
        if(is.good())
        {
            std::string help;
            help.assign((std::istreambuf_iterator<char>(is)),
                std::istreambuf_iterator<char>());
            is.close();
            DocOpts opts;
            if (opts.process(help.c_str(), argc - 1, &argv[1]))
            {
                opts.toCmd();
                ret = 0;
            }
        }
    }
    return ret;
}

