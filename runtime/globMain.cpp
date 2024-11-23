#include <SOM/Glob.h>
#include <SOM/GlobProcessors.h>
#include <SOM/docopts.h>

#include <filesystem>
#include <iostream>
#include <memory>

const CONST_C_STRING cOpts =
    "options:\n"
    "-x  <command> command to execute with each arg\n"
    "-p  <placeholder> placeholder for command\n"
    "    default: <ARG>\n"
    "-f  files only\n"
    "-d  directories only\n"
    "-h  this help\n"
;

void help(const CONST_C_STRING arg)
{
    std::cout
        << "\nglob\n\n"
        << "usage: " << std::filesystem::path(arg).filename().string() << " [options] args\n"
        << cOpts;
    ;
}

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    auto ret = 1;
    DocOpts opts;
    if (opts.process(cOpts, argc, argv))
    {
        if (opts.isSet('h'))
        {
            help(argv[0]);
        }
        else
        {
            ret = 0;
            std::unique_ptr<I_GlobProcessor> proc;
            CONST_C_STRING cmd = nullptr;
            if (opts.getValue(cmd, 'x'))
            {
                CONST_C_STRING placeholder = nullptr;
                opts.getValue(placeholder, 'p');
                proc = std::make_unique<GlobXargs>(cmd, placeholder);
            }
            else
            {
                proc = std::make_unique<GlobArgs>();
            }
            Glob glob(*proc, opts.isSet('f'), opts.isSet('d'));
            for (auto i = 0; i < opts.argc(); ++i)
            {
                glob.glob(opts.args()[i]);
            }
        }
    }
    return ret;
}
