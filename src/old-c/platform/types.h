#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

#ifdef _WIN32
    #define FL "%ll"
#else
    #define FL "%l"
#endif

#define ASSERT(E) ( (E) ? 0 : (log__printf("[ASSERT] %s:%d %s\n", __FILE__, __LINE__, #E), *((int*)0) = 5) )
