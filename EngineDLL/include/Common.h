#pragma once

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char      i8;
typedef signed short     i16;
typedef signed int       i32;
typedef signed long long i64;

typedef float  f32;
typedef double f64;

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#ifndef DO_NLOG
#include <stdarg.h>
#include "Utils/Logger.h"
#endif // DO_NLOG

#ifdef DEBUG
#include "Utils/DebugMemory.h"
#endif // DEBUG

#ifdef DEBUG
#define DO_ALLOC(T, N) _DebugMalloc(sizeof(T) * (N), __FILE__, __LINE__)
#define DO_FREE(ptr) _DebugFree(ptr, __FILE__, __LINE__)
#define DO_PRINT_LEAKS() _DebugPrintLeaks()
#define DO_CLEANUP() _DebugCleanup()
#else
#define DO_ALLOC(T, N) malloc(sizeof(T) * (N))
#define DO_FREE(ptr) free(ptr)
#define DO_PRINT_LEAKS()
#define DO_CLEANUP()
#endif // DEBUG

#define DO_ZERO_MEM(ptr, N) memset(ptr, 0, sizeof(*ptr) * N)

#ifdef DEBUG
#ifdef _MSC_VER
#define DO_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DO_DEBUG_BREAK() __builtin_trap()
#else
#define DO_DEBUG_BREAK() (*(volatile int*) 0 = 0)
#endif // _MSC_VER

#define DO_DEBUG_OP(x) x

#define DO_TRACE(...) _Log("[TRACE]", __FILE__, __LINE__, TEXT_COLOR_GREEN, __VA_ARGS__)
#define DO_WARN(...) _Log("[WARN]", __FILE__, __LINE__, TEXT_COLOR_YELLOW, __VA_ARGS__)
#define DO_ERROR(...) _Log("[ERROR]", __FILE__, __LINE__, TEXT_COLOR_RED, __VA_ARGS__)

#define DO_ASSERT(x) \
    if (!(x)) DO_DEBUG_BREAK();

#define DO_ASSERT_MSG(x, ...)  \
    if (!(x))                  \
    {                          \
        DO_ERROR(__VA_ARGS__); \
        DO_DEBUG_BREAK();      \
    }

#else
#define DO_DEBUG_BREAK()

#define DO_DEBUG_OP(x)

#define DO_TRACE(...)
#define DO_WARN(...)
#define DO_ERROR(...)

#define DO_ASSERT(x)
#define DO_ASSERT_MSG(x, ...)
#endif // DEBUG

#include "Utils/ArenaAllocator.h"

#define BIT(x) (1 << x)
#define KB(x) (x * 1024)
#define MB(x) (KB(x) * 1024)

#define DO_PERSISTENT &g_memory->persistentStorage
#define DO_TRANSIENT &g_memory->transientStorage

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

// #define RELEASE(x) x->lpVtbl->Release(x)
// #define SAFE_RELEASE(x) if (x) x->lpVtbl->Release(x)
