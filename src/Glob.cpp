#include <SOM/Glob.h>
#include <SOM/pyregex.h>

#define TRACE_ME
#include <SOM/TraceMacros.h>

#include <iostream>
#include <regex>

using
    py::repl,
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

Glob::Glob(I_GlobProcessor& proc, const bool onlyFiles, const bool onlyDirs):
    mFiles(onlyFiles or (not onlyDirs)),
    mDirs(onlyDirs or (not onlyFiles)),
    mProc(proc)
{}

bool Glob::isGlob(const string& token)
{
    static const regex reGlob("\\[.*\\]|[?*]");
    return regex_search(token, reGlob);
}

void Glob::tokenize(strVec& v, const string& path)
{
    TRACE_FUNC()
    static const regex rxTok("([^/\\\\]+)[/\\\\]?");
    static const regex rxTop("^[/\\\\]");
    std::smatch sm;
    if (regex_search(path, sm, rxTop))
    {
        v.push_back(sm[0]);
    }
    string::const_iterator ps(path.cbegin());
    while (regex_search(ps, path.cend(), sm, rxTok))
    {
        v.push_back(sm[1]);
        ps = sm.suffix().first;
    }
}

void Glob::callProc(const fspath& path)
{
    TRACE_FUNC()
    if (
        (mFiles and is_regular_file(path)) or
        (mDirs and is_directory(path))
    )
    {
        mProc.process(path.string());
    }
}

void Glob::procDirs(const fspath& path, const size_t pos, const bool isLast, const bool recursive)
{
    TRACE_FUNC()
    for (const auto& e : directory_iterator(path))
    {
        const auto& ep = e.path();
        if (isLast) callProc(ep);
        else if (is_directory(e)) process(ep, pos + 1);

        if (recursive and is_directory(e)) procDirs(ep, pos, isLast, recursive);
    }
}

void Glob::procAll(const fspath& path)
{
    TRACE_FUNC()
    for (const auto& e : directory_iterator(path))
    {
        callProc(e.path());
    }
}

void Glob::procGlob(const fspath& path, size_t pos, bool isLast)
{
    TRACE_FUNC()
    static const regex reAst("[*]");
    static const regex reDot("[.]");
    static const regex reQue("[?]");
    const string& item = mItems[pos];
    const regex re(repl(reQue, ".", repl(reAst, ".*", repl(reDot, "\\.", item))));
    for (const auto& e : directory_iterator(path))
    {
        const auto& ep = e.path();
        if (regex_match(ep.filename().string(), re))
        {
            if (isLast) callProc(ep);
            else if (is_directory(e)) process(ep, pos + 1);
        }
    }
}

void Glob::process(const fspath& path, const size_t pos)
{
    TRACE_FUNC()
    const bool isLast = (pos == mItems.size() - 1);
    const string& item = mItems[pos];

    //  path for directory globbing
    static const fspath pCurr(".");
    const fspath& gp = path.empty() ? pCurr : path;

    if (item == "**")
    {
        if (mDirs or (not isLast))
            procDirs(gp, pos, isLast, true);
    }
    else if (item == "*")
    {
        if (isLast)
            procAll(gp);
        else
            procDirs(gp, pos);
    }
    else if (isGlob(item))
    {
        procGlob(gp, pos, isLast);
    }
    else
    {
        try
        {
            const fspath p =
                (not path.empty()) ?
                    path / item :
                    item == "~" ? fspath(getHome()) :
                    fspath(item)
            ;
            TRACE_VAR(p)
            if (exists(p))
            {
                if (isLast) callProc(p);
                else if (is_directory(p)) process(p, pos + 1);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
}

void Glob::glob(const string& fpath)
{
    TRACE_FUNC()
    if (not todo(fpath))
    {
        const fspath p { fpath };
        if (exists(p)) callProc(p);
        return;
    }
    mItems.clear();
    tokenize(mItems, fpath);
    if (mItems.empty()) return;
    process(fspath(), 0);
}

CONST_C_STRING Glob::getHome()
{
    TRACE_FUNC()
    #ifdef _WIN32
        #define C_HOME "USERPROFILE"
    #else
        #define C_HOME "HOME"
    #endif

    const CONST_C_STRING home = getenv(C_HOME);
    if (home == nullptr)
    {
        throw std::invalid_argument("error: env " C_HOME " not set");
    }
    return home;
}
