#pragma once


// TODO: Add linux support
#ifdef HORYZEN_WINDOWS_BUILD
	#ifdef HORYZEN_BUILD
		#define HORYZEN_OBJECT __declspec(dllexport)
	#else
		#define HORYZEN_OBJECT __declspec(dllimport)
	#endif
#endif


// Assertions
#ifdef HORYZEN_ENABLE_ASSERTIONS
	#define HORYZEN_ASSERT(x, ...) {if (!(x)) {HORYZEN_LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define APP_ASSERT(x, ...)     {if (!(x)) {APP_LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define HORYZEN_ASSERT(x, ...)
	#define APP_ASSERT(x, ...)
#endif


#include <cstdint>

// Typedefs for basic int and float types that we are going to use everywhere
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


typedef float  f32;
typedef double f64;





