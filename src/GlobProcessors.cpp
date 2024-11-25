#include <SOM/GlobProcessors.h>

#define TRACE_ME
#include <SOM/TraceMacros.h>

#include <iostream>
using std::cout;

void GlobTrace::process(const std::string& fpath)
{
    TRACE_VAR(fpath)
}

GlobXargs::GlobXargs(CONST_C_STRING cmd, CONST_C_STRING placeholder):
    cmd(cmd),
    re(placeholder ? placeholder : "<ARG>")
{}

void GlobXargs::process(const std::string& fpath)
{
    cout << std::regex_replace(cmd, re, fpath) << '\n';
}

GlobArgs::GlobArgs()
{
#ifdef _WIN32
        cout << "set _args=";
#else
        cout << "export _args=\"";
#endif
}

GlobArgs::~GlobArgs()
{
#ifndef _WIN32
    cout << '"';
#endif
    cout << '\n';
}

void GlobArgs::process(const std::string& fpath)
{
    cout << fpath << ' ';
}
