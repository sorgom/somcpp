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

class I_FglobProcessor
{
public:
    virtual void process(CONST_C_STRING fpath) = 0;
};

//!  glob files
//!  @param item file, or directory with file glob pattern
//!  @param proc I_FglobProcessor
#ifdef _WIN32
void fglob(CONST_C_STRING item, I_FglobProcessor& proc);
#else
inline void fglob(CONST_C_STRING item, I_FglobProcessor& proc)
{
    proc.process(item);
}
#endif // _WIN32

#endif // _H
