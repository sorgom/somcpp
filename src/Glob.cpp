#include <SOM/Glob.h>
#include <SOM/pyregex.h>
#include <SOM/TraceMacros.h>

#include <algorithm>
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
    onlyDirs(onlyDirs),
    proc(proc),
    filter(onlyFiles ? ff : onlyDirs ? fd : fx)
{}

const Glob::ffunc Glob::fd = [](const string& s) { return is_directory(s); };
const Glob::ffunc Glob::ff = [](const string& s) { return is_regular_file(s); };
const Glob::ffunc Glob::fx = [](const string& s) { return exists(s); };

void Glob::pathVec::add(const string& s)
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

void Glob::pathVec::add(const std::filesystem::directory_entry& e)
{
    push_back(e.path().string());
}

bool Glob::isGlob(const string& token)
{
    static const regex reGlob("\\[.*\\]|[?*]");
    return regex_search(token, reGlob);
}

void Glob::tokenize(strVec& v, const string& path)
{
    TRACE_FUNC()
    static const regex rxTok("[/\\\\]");
    //  regex applied on string&& deleted in Windows
    //  therefore operate const char* instead
    const CONST_C_STRING c = path.c_str();
    std::cmatch cm;
    size_t pos = 0;
    while (regex_search(c + pos, cm, rxTok))
    {
        TRACE_VAR(pos)
        const size_t p = cm.position();
        v.push_back(path.substr(pos, p));
        pos += (p + 1);
    }
    if (pos < path.size())
    {
        v.push_back(path.substr(pos));
    }
}

void Glob::getAll()
{
    TRACE_FUNC()
    const pathVec& src = getSrc();
    pathVec& trg = getTrg();
    for (const auto& path: src)
    {
        if (is_directory(path))
        {
            for (const auto& e : directory_iterator(fspath(path)))
            {
                trg.add(e);
            }
        }
    }
}

void Glob::getDirs(const pathVec& src, const bool recursive)
{
    TRACE_FUNC()
    TRACE_VAR(recursive)
    pathVec next;
    pathVec& trg = getTrg();
    for (const auto& path: src)
    {
        if (is_directory(path))
        {
            for (const auto& e : directory_iterator(fspath(path)))
            {
                if (is_directory(e))
                {
                    trg.add(e);
                    if (recursive) next.add(e);
                }
            }
        }
    }
    if (recursive and not next.empty()) getDirs(next, true);
}

void Glob::getGlob(const string& token, const bool dirs)
{
    TRACE_FUNC()
    TRACE_VAR(dirs)
    static const regex reAst("[*]");
    static const regex reDot("[.]");
    static const regex reQue("[?]");
    const regex re(repl(reQue, ".", repl(reAst, ".*", repl(reDot, "\\.", token))));
    const pathVec& src = getSrc();
    pathVec& trg = getTrg();
    for (const auto& path: src)
    {
        if (is_directory(path))
        {
            for (const auto& e : directory_iterator(fspath(path)))
            {
                if (
                    ((not dirs) or is_directory(e)) and
                    regex_match(e.path().filename().string(), re)
                )
                {
                    trg.add(e);
                }
            }
        }
    }
}

void Glob::swap()
{
    std::swap(pSrc, pTrg);
    getTrg().clear();
}

void Glob::glob(const string& fpath)
{
    TRACE_FUNC()
    strVec tokens;
    tokenize(tokens, fpath);
    size_t n = 0;
    const size_t sz = tokens.size();
    for (const auto& token : tokens)
    {
        ++n;
        TRACE_VAR(sz)
        TRACE_VAR(n)
        TRACE_VAR(token)
        if (token == "**")
        {
            getDirs(getSrc(), true);
            swap();
        }
        else if (token == "*")
        {
            if (onlyDirs or n < sz)
                getDirs(getSrc(), false);
            else
                getAll();
            swap();
        }
        else if (isGlob(token))
        {
            getGlob(token, onlyDirs or n < sz);
            swap();
        }
        else
        {
            getSrc().add(token);
        }
    }
    for (const string& s: getSrc())
    {
        if (filter(s)) proc.process(s);
    }
}
