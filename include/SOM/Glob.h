#pragma once
#ifndef GLOB_H
#define GLOB_H

#include <SOM/BaseTypes.h>
#include <SOM/coding.h>

#include <filesystem>
#include <string>
#include <vector>

class I_GlobProcessor
{
public:
    virtual void process(const std::string& fpath) = 0;
    virtual ~I_GlobProcessor() = default;
};

class Glob
{
public:
    Glob(I_GlobProcessor& proc, bool onlyFiles=false, bool onlyDirs=false);
#if defined(_WIN32) or defined(GLOB_LINUX)
    void glob(const CONST_C_STRING fpath);
#else
    inline void glob(const CONST_C_STRING fpath)
    {
        callProc(fspath(fpath));
    }
#endif // _WIN32
    void glob(INT32 argc, const CONST_C_STRING* argv, INT32 start=0);

private:
    using fspath = std::filesystem::path;
    static bool isGlob(const CONST_C_STRING token);
    inline static bool todo(const CONST_C_STRING fpath)
    {
        return isGlob(fpath) or fpath[0] == '~';
    }
    const bool mFiles;
    const bool mDirs;
    I_GlobProcessor& mProc;

#if defined(_WIN32) or defined(GLOB_LINUX)
    using strVec = std::vector<std::string>;
    strVec mItems;
    static void tokenize(strVec& tokens, const CONST_C_STRING fpath);
    void process(const fspath& path, size_t pos);
    inline void process(const fspath&& path, size_t pos) { process(path, pos); }
    void procDirs(const fspath& path, size_t pos, bool isLast=false, bool recursive=false);
    void procGlob(const fspath& path, size_t pos, bool isLast=false);
    void procAll(const fspath& path);
    static CONST_C_STRING getHome();
#endif // _WIN32
    void callProc(const fspath& path);

    NODEF(Glob)
    NOCOPY(Glob)
};
#endif // _H
