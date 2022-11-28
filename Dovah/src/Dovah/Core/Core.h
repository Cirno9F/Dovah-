#pragma once

#include <memory>

#ifdef DOVAH_PLATFORM_WINDOWS
#if HZ_DYNAMIC_LINK
	#ifdef DOVAH_BUILD_DLL
		#define DOVAH_API _declspec(dllexport)
	#else	
		#define DOVAH_API _declspec(dllimport)
	#endif
#else
	#define DOVAH_API
#endif
#else
	#error Doavh only support Windows!
#endif

#ifdef DOVAH_DEBUG
	#define DOVAH_ENABLE_ASSERTS
#endif

#ifdef DOVAH_ENABLE_ASSERTS
	#define DOVAH_ASSERT(x, ...) { if(!(x)) { DOVAH_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
	#define DOVAH_CORE_ASSERT(x, ...) { if(!(x)) { DOVAH_CORE_ERROR("Assertion Failed: {0}",__VA_ARGS__); __debugbreak(); } }
#else
	#define DOVAH_ASSERT(x, ...)
	#define DOVAH_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define DOVAH_BIND_EVENT_FN(fn) std::bind(&fn,this,std::placeholders::_1)

namespace Dovah
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}