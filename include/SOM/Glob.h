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
    void glob(const std::string& fpath);

private:
    using strVec = std::vector<std::string>;
    using fspath = std::filesystem::path;
    static bool isGlob(const std::string& token);
    static void tokenize(strVec& tokens, const std::string& fpath);

    const bool mFiles;
    const bool mDirs;
    I_GlobProcessor& mProc;
    strVec mItems;
    void process(const fspath& path, size_t pos);
    inline void process(const fspath&& path, size_t pos) { process(path, pos); }
    void procDirs(const fspath& path, size_t pos, bool isLast=false, bool recursive=false);
    void procGlob(const fspath& path, size_t pos, bool isLast=false);
    void procAll(const fspath& path);
    void callProc(const fspath& path);

    NOCOPY(Glob)
};
#endif // _H
