#include <SOM/docopts.h>
#include <SOM/fio.h>

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    INT32 ret = 1;
    std::string help;
    if (
        argc > 1 and
        read(help, argv[1], false)
    )
    {
        DocOpts opts(help.c_str());
        if (opts.process(argc, argv, 2))
        {
            opts.toShell();
            ret = 0;
        }
    }
    return ret;
}
