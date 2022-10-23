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

#define BIT(x) (1 << x)