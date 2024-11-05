//  ============================================================
//  end of packed alignment for data structs
//  ============================================================

//  prevent include guards to be inserted by script
#ifndef PACKEND_H
#define PACKEND_H
#endif // _H

//! check if packBegin.h has been included before
#ifdef __IS_PACKED__
    #undef __IS_PACKED__
    //  this works with GCC and VS
    #pragma pack (pop)
#else
    #error Missing previous #include "packBegin.h"
#endif
