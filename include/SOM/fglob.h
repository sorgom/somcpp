//  ============================================================
//  a very simple file name globbing utility for windows
//  intended to be used for main arguments
//  - supports * [0-9] ? in file names
//  - does not support any directory globbing
//  ============================================================
//  created by Manfred Sorgo

#pragma once
#ifndef FGLOB_H
#define FGLOB_H

#include <SOM/BaseTypes.h>
#include <string>

class I_FglobProcessor
{
public:
    virtual void process(const std::string& fpath) = 0;
};

//!  glob files
//!  @param item file, or directory with file glob pattern
//!  @param proc I_GlobProcessor
#ifdef _WIN32
void fglob(const std::string& item, I_FglobProcessor& proc);
#else
inline void fglob(const std::string& item, I_FglobProcessor& proc)
{
    proc.process(item);
}
#endif // _WIN32

#endif // _H
