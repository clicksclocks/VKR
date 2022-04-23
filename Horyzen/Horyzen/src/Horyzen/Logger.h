#pragma once

#pragma 
#include "pchheader.h"

#include "Core.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

namespace Horyzen {

	class HORYZEN_OBJECT Logger
	{
	public:

		static void Init();

		static std::shared_ptr<spdlog::logger>& GetEngineLogger();
		static std::shared_ptr<spdlog::logger>& GetAppLogger();

	private:
		
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_AppLogger;

	};

}


#if defined(HORYZEN_DEBUG_BUILD) || defined(HORYZEN_RELEASE_BUILD)
	#define HORYZEN_LOG_TRACE(...)    Horyzen::Logger::GetEngineLogger()->trace(__VA_ARGS__)
	#define HORYZEN_LOG_INFO(...)     Horyzen::Logger::GetEngineLogger()->info(__VA_ARGS__)
	#define HORYZEN_LOG_WARN(...)     Horyzen::Logger::GetEngineLogger()->warn(__VA_ARGS__)
	#define HORYZEN_LOG_ERROR(...)    Horyzen::Logger::GetEngineLogger()->error(__VA_ARGS__)
	#define HORYZEN_LOG_CRITICAL(...) Horyzen::Logger::GetEngineLogger()->critical(__VA_ARGS__)

	#define APP_LOG_TRACE(...)    Horyzen::Logger::GetAppLogger()->trace(__VA_ARGS__)
	#define APP_LOG_INFO(...)     Horyzen::Logger::GetAppLogger()->info(__VA_ARGS__)
	#define APP_LOG_WARN(...)     Horyzen::Logger::GetAppLogger()->warn(__VA_ARGS__)
	#define APP_LOG_ERROR(...)    Horyzen::Logger::GetAppLogger()->error(__VA_ARGS__)
	#define APP_LOG_CRITICAL(...) Horyzen::Logger::GetAppLogger()->critical(__VA_ARGS__)
#else
	#define HORYZEN_LOG_TRACE   
	#define HORYZEN_LOG_INFO    
	#define HORYZEN_LOG_WARN    
	#define HORYZEN_LOG_ERROR   
	#define HORYZEN_LOG_CRITICAL

	#define APP_LOG_TRACE 
	#define APP_LOG_INFO    
	#define APP_LOG_WARN    
	#define APP_LOG_ERROR   
	#define APP_LOG_CRITICAL
#endif