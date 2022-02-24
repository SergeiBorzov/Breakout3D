#ifndef FLY_PLATFORM_H
#define FLY_PLATFORM_H

#ifdef FLY_COMPILING_DLL
#define FLY_API __declspec(dllexport)
#else
#define FLY_API __declspec(dllimport)
#endif

#endif /* End of FLY_PLATFORM_H */
