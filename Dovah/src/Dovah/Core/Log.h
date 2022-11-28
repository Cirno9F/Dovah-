#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Dovah {
	class DOVAH_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define DOVAH_CORE_TRACE(...) Dovah::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define DOVAH_CORE_INFO(...)  Dovah::Log::GetCoreLogger()->info(__VA_ARGS__)
#define DOVAH_CORE_WARN(...)  Dovah::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define DOVAH_CORE_ERROR(...) Dovah::Log::GetCoreLogger()->error(__VA_ARGS__)
#define DOVAH_CORE_FATAL(...) Dovah::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define DOVAH_TRACE(...) Dovah::Log::GetClientLogger()->trace(__VA_ARGS__)
#define DOVAH_INFO(...)  Dovah::Log::GetClientLogger()->info(__VA_ARGS__)
#define DOVAH_WARN(...)  Dovah::Log::GetClientLogger()->warn(__VA_ARGS__)
#define DOVAH_ERROR(...) Dovah::Log::GetClientLogger()->error(__VA_ARGS__)
#define DOVAH_FATAL(...) Dovah::Log::GetClientLogger()->fatal(__VA_ARGS__)


