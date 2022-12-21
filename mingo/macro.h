#ifndef MINGO_MACRO_H
#define MINGO_MACRO_H

#include <string>
#include <assert.h>
#include "log.h"
#include "config.h"

#if defined __GNUC__ || defined __llvm__
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率成立
#   define MINGO_LIKELY(x)       __builtin_expect(!!(x), 1)
/// LIKCLY 宏的封装, 告诉编译器优化,条件大概率不成立
#   define MINGO_UNLIKELY(x)     __builtin_expect(!!(x), 0)
#else
#   define MINGO_LIKELY(x)      (x)
#   define MINGO_UNLIKELY(x)      (x)
#endif

/// 断言宏封装
#define MINGO_ASSERT(x) \
    if(MINGO_UNLIKELY(!(x))) { \
        LOG_ERROR(GET_LOGGER("system"), mingo::BacktraceToString(100, 2, "    ")); \
        assert(x); \
    }

/// 断言宏封装
#define MINGO_ASSERT2(x, w) \
    if(MINGO_UNLIKELY(!(x))) { \
        LOG_ERROR(GET_LOGGER("system"), #w\
            + mingo::BacktraceToString(100, 2, "    ")); \
        assert(x); \
    }

#endif
