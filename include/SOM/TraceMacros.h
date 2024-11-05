#pragma once
#ifndef TRACE_MACROS_H
#define TRACE_MACROS_H

#if defined(TRACE_ALL) || (defined(TRACE_ON) && defined(TRACE_ME))

#include <iostream>
#include <iomanip>
#define TRACE(MSG) std::cout << MSG << '\n';
#define TRACE_ERR(MSG) std::cerr << MSG << '\n';
#define TRACE_VAR(VAR) TRACE(#VAR << ": " << VAR)

#include <chrono>

using _TracePoint = std::chrono::time_point<std::chrono::steady_clock>;

struct T_Trace
{
    const char* const mSection;
    const _TracePoint mStart;
    inline T_Trace(const char* section):
        mSection(section),
        mStart(std::chrono::steady_clock::now())
    {}
    inline ~T_Trace()
    {
        const _TracePoint end = std::chrono::steady_clock::now();
        TRACE('[' << std::setw(5) << std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count() << "] " << mSection);
    }
};
struct F_Trace
{
    const char* const mSection;
    inline F_Trace(const char* section):
        mSection(section)
    {
        TRACE(">> " << section)
    }
    inline ~F_Trace()
    {
        TRACE("<< " << mSection)
    }
};

//  compiler specifica
#if defined(COMPILER_CADUL)
    #define TRACE_FUNC_TIME()
    #define TRACE_FUNC()
#elif defined(_MSC_VER)
    #define TRACE_FUNC_TIME() T_Trace __T_TRACE__ ## __LINE__ (__FUNCTION__);
    #define TRACE_FUNC() F_Trace __F_TRACE__ ## __LINE__ (__FUNCTION__);
#else
    #define TRACE_FUNC_TIME() T_Trace __T_TRACE__ ## __LINE__ (__func__);
    #define TRACE_FUNC() F_Trace __F_TRACE__ ## __LINE__ (__func__);
#endif

#define TRACE_FLOW_TIME(WHATEVER) T_Trace __T_TRACE__ ## __LINE__ (#WHATEVER);
#define TRACE_FLOW(WHATEVER) F_Trace __F_TRACE__ ## __LINE__ (#WHATEVER);

#else
#define TRACE(MSG)
#define TRACE_ERR(MSG)
#define TRACE_VAR(VAR)
#define TRACE_FUNC()
#define TRACE_FLOW(WHATEVER)
#define TRACE_FUNC_TIME()
#define TRACE_FLOW_TIME(WHATEVER)
#endif  //  TRACE active

#endif // _H
