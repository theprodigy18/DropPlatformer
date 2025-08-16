#pragma once

#ifdef DO_DLL_EXPORTS
#define DO_API __declspec(dllexport)
#else
#define DO_API __declspec(dllimport)
#endif // DO_DLL_EXPORTS

DO_API int EntryPoint();