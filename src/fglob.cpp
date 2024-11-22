#ifdef _WIN32
#include <SOM/fglob.h>
#include <SOM/pyregex.h>
#include <SOM/coding.h>

// #include <algorithm>
#include <filesystem>
#include <regex>

using py::repl;
using fp = std::filesystem::path;
using std::regex, std::regex_replace, std::regex_match;
using std::string;

void fglob(const string& item, I_FglobProcessor& proc)
{
    //  any posix system has expanded globbing
    if (std::filesystem::is_regular_file(item))
    {
        proc.process(item);
    }
    //  windows crap shell has no globbing
    //  therefore at least apply simple file globbing by regex
    else if (string(item).find('*') != string::npos)
    {
        // simple globbing:
        // abc*[0-9]?.txt -> abc.*[0-9].\.txt
        // . -> \.
        // * -> .*
        // ? -> .
        static const regex reAst("[*]");
        static const regex reDot("[.]");
        static const regex reQue("[?]");

        const string restr = repl(reQue, ".", repl(reAst, ".*", repl(reDot, "\\.", fp(item).filename().string())));
        const regex re(restr);
        for (const auto& entry : std::filesystem::directory_iterator(fp(item).parent_path()))
        {
            if (
                entry.is_regular_file() and
                regex_match(entry.path().filename().string(), re)
            )
            {
                proc.process(entry.path().string().c_str());
            }
        }
    }
}
#endif // _WIN32
