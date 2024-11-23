#pragma once
#ifndef GLOBPROCESSORS_H
#define GLOBPROCESSORS_H

#include "Glob.h"
#include <SOM/BaseTypes.h>
#include <SOM/coding.h>

#include <regex>

class GlobTrace : public I_GlobProcessor
{
public:
    void process(const std::string& fpath) override;
};

class GlobXargs : public I_GlobProcessor
{
public:
    GlobXargs(CONST_C_STRING cmd, CONST_C_STRING placeholder=nullptr);
    void process(const std::string& fpath) override;
private:
    const std::string cmd;
    const std::regex re;
    NODEF(GlobXargs)
    NOCOPY(GlobXargs)
};

class GlobArgs : public I_GlobProcessor
{
public:
    GlobArgs();
    ~GlobArgs();
    void process(const std::string& fpath) override;
};

#endif // _H
