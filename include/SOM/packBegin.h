//  ============================================================
//  begin of packed alignment for data structs
//  ============================================================

//  prevent include guards to be inserted by script
#ifndef PACKBEGIN_H
#define PACKBEGIN_H
#endif // _H

//! check if we have an nested Pack definition
#ifdef __IS_PACKED__
    #error Nested use of PackBegin.h (probably missing #include "packEnd.h")
#else
    //  this works with GCC and VS
    #pragma pack (push, 1)
    #define __IS_PACKED__
#endif
