#pragma once
#ifndef GLOB_H
#define GLOB_H

#include <SOM/BaseTypes.h>
#include <SOM/coding.h>
#include <SOM/fglob.h>

#include <filesystem>
#include <functional>
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
    struct pathVec : strVec
    {
        void add(const std::string& s);
        void add(const std::filesystem::directory_entry& e);
    };

    using ffunc = std::function<bool(const std::string&)>;
    static const ffunc fd;
    static const ffunc ff;
    static const ffunc fx;
    static bool isGlob(const std::string& token);
    static void tokenize(strVec& tokens, const std::string& fpath);

    const bool onlyDirs;
    I_GlobProcessor& proc;
    const ffunc filter;
    pathVec buffs[2];
    UINT8 pSrc = 0;
    UINT8 pTrg = 1;
    inline pathVec& getSrc() { return buffs[pSrc]; }
    inline pathVec& getTrg() { return buffs[pTrg]; }
    void getAll();
    void getDirs(const pathVec& src, bool recursive=false);
    void getGlob(const std::string& token, bool dirs=false);
    void swap();

    NOCOPY(Glob)
};
#endif // _H
