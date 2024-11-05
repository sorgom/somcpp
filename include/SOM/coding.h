//  ============================================================
//  misc coding enhancers
//  ============================================================
//  created by Manfred Sorgo
#pragma once
#ifndef CODING_H
#define CODING_H

//  enable usage of "and", "or", "not" in place of "&&", "||", "!"
#include <ciso646>

//  unified instance declaration
#define INSTANCE_DEC(NAME) \
    static NAME& instance();

//  unified instance definition
#define INSTANCE_DEF(NAME) \
    NAME& NAME::instance() \
    { \
        static NAME instance; \
        return instance; \
    }

//  unified instance declaration
#define CONST_INSTANCE_DEC(NAME) \
    static const NAME& instance();

//  unified instance definition
#define CONST_INSTANCE_DEF(NAME) \
    NAME& NAME::instance() \
    { \
        static const NAME instance; \
        return instance; \
    }

//  disable copy constructor and copy operator
#define NOCOPY(CLASS) \
CLASS(const CLASS&) = delete; \
CLASS(CLASS&&) = delete; \
CLASS& operator = (const CLASS&) = delete;

//  disable default constructor
#define NODEF(CLASS) \
CLASS() = delete;


#endif // _H
