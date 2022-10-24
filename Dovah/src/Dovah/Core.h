#pragma once

#ifdef DOVAH_PLATFORM_WINDOWS
	#ifdef DOVAH_BUILD_DLL
		#define DOVAH_API _declspec(dllexport)
	#else	
		#define DOVAH_API _declspec(dllimport)
	#endif
#else
	#error Doavh only support Windows!
#endif

#ifdef DOVAH_ENABLE_ASSERTS
	#define DOVAH_ASSERT(x, ...) { if(!(x)) { DOVAH_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
	#define DOVAH_CORE_ASSERT(x, ...) { if(!(x)) { DOVAH_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
#else
	#define DOVAH_ASSERT(x, ...)
	#define DOVAH_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)