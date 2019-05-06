#pragma once
#ifdef SUBSMATH_IN_CPPTL 
#define SUBSMATH_API CPPTL_API
#elif defined(SUBSMATH_DLL_BUILD) /// DLL BUILD
#if defined(_MSC_VER) || defined(__MINGW32__)
#define SUBSMATH_API __declspec(dllexport)
#define SUBSMATH_API_GEN_STL
#endif // if defined(_MSC_VER)
#elif defined(SUBSMATH_DLL) /// DYNAMIC LINK
#if defined(_MSC_VER) || defined(__MINGW32__)
#define SUBSMATH_API __declspec(dllimport)
#define SUBSMATH_API_GEN_STL extern
#endif // if defined(_MSC_VER)
#else /// STATIC LINK
#define SUBSMATH_API
#define SUBSMATH_API_GEN_STL 
#endif // ifdef SUBSMATH_IN_CPPTL
