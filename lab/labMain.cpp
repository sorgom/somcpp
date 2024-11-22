#include <SOM/BaseTypes.h>
#include <SOM/TraceMacros.h>
#include <SOM/pyregex.h>
#include <SOM/fglob.h>
using py::repl;
// #include <iostream>

#include <filesystem>
#include <regex>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ciso646>

// using namespace std;
using
    std::filesystem::exists,
    std::filesystem::is_directory,
    std::filesystem::is_regular_file,
    std::filesystem::directory_iterator,
    std::regex_match,
    std::regex_search,
    std::regex,
    std::string,
    std::vector
;

using fspath = std::filesystem::path;
using tokenVec = vector<string>;

struct pathVec : vector<string>
{
    void add(const string& s)
    {
        if (empty()) push_back(s);
        else
        {
            for (auto& str: *this)
            {
                str += '/' + s;
            }
        }
    }

    inline pathVec& operator << (const std::filesystem::directory_entry& e)
    {
        push_back(e.path().string());
        return *this;
    }
};

void getAll(pathVec& trg, const pathVec& src, bool recursive=false)
{
    TRACE_FUNC()
    pathVec next;
    for (const auto& path: src)
    {
        if (is_directory(path))
        {
            for (const auto& e : directory_iterator(fspath(path)))
            {
                    trg << e;
                    if (recursive and is_directory(e)) next << e;
            }
        }
    }
    if (recursive and not next.empty()) getAll(trg, next, true);
}

void getRegex(pathVec& trg, const pathVec& src, const regex&& re)
{
    TRACE_FUNC()
    for (const auto& path: src)
    {
        if (is_directory(path))
        {
            for (const auto& e : directory_iterator(fspath(path)))
            {
                if (regex_match(e.path().filename().string(), re))
                {
                    trg << e;
                }
            }
        }
    }
}

void glob(I_FglobProcessor& proc, const string& fpath, const bool onlyFiles=false, const bool onlyDirs=false)
{
    static const regex reGlob("\\[.*\\]|[?*]");
    static const regex reAst("[*]");
    static const regex reDot("[.]");
    static const regex reQue("[?]");
    static const regex rxTok("[/\\\\]");

    static const auto isGlob = [](const string& token) -> bool
    {
        return regex_search(token, reGlob);
    };

    static const auto toRegex = [](const string& token) -> regex
    {
        return regex(repl(reQue, ".", repl(reAst, ".*", repl(reDot, "\\.", token))));
    };

    static const auto tokenize = [](tokenVec& v, const string& path)
    {
        string s = path;
        std::smatch sm;
        while (regex_search(s, sm, rxTok))
        {
            v.push_back(s.substr(0, sm.position()));
            s = s.substr(sm.position() + 1);
        }
        v.push_back(s);
    };

    pathVec buffs[2];
    auto pSrc = 0, pTrg = 1;

    const auto getSrc = [&]() -> pathVec& { return buffs[pSrc]; };
    const auto getTrg = [&]() -> pathVec& { return buffs[pTrg]; };

    const auto swap = [&]() {
        std::swap(pSrc, pTrg);
        getTrg().clear();
    };

    tokenVec tokens;
    tokenize(tokens, fpath);

    for (const auto& token : tokens)
    {
        TRACE_VAR(token)
        if (token == "**")
        {
            getAll(getTrg(), getSrc(), true);
            swap();
        }
        else if (token == "*")
        {
            getAll(getTrg(), getSrc(), false);
            swap();
        }
        else if (isGlob(token))
        {
            getRegex(getTrg(), getSrc(), toRegex(token));
            swap();
        }
        else
        {
            getSrc().add(token);
        }
    }

    for (const string& s: getSrc())
    {
        if (onlyDirs)
        {
            if (is_directory(s)) proc.process(s);
        }
        else if (onlyFiles)
        {
            if (is_regular_file(s)) proc.process(s);
        }
        else if (exists(s))
        {
            proc.process(s);
        }
    }
}

struct TraceFglob : I_FglobProcessor
{
    inline void process(const string& fpath) override
    {
        TRACE_VAR(fpath)
    }
};

INT32 main(const INT32 argc, const CONST_C_STRING* const argv)
{
    TraceFglob tfg;
    if (argc > 1)
    {
        glob(tfg, argv[1]);
    }
    return 0;
}
