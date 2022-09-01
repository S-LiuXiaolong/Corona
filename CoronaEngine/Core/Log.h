#pragma once

#include "crpch.h"

#include "GameCore.h"
#include "spdlog/spdlog.h"

// TODO: Log system will be integrated into imgui console
namespace Corona {

	class Log
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

// Core log macros
#define CR_CORE_TRACE(...)    ::Corona::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CR_CORE_INFO(...)     ::Corona::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CR_CORE_WARN(...)     ::Corona::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CR_CORE_ERROR(...)    ::Corona::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CR_CORE_FATAL(...)    ::Corona::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define CR_TRACE(...)	      ::Corona::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CR_INFO(...)	      ::Corona::Log::GetClientLogger()->info(__VA_ARGS__)
#define CR_WARN(...)	      ::Corona::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CR_ERROR(...)	      ::Corona::Log::GetClientLogger()->error(__VA_ARGS__)
#define CR_FATAL(...)	      ::Corona::Log::GetClientLogger()->fatal(__VA_ARGS__)

