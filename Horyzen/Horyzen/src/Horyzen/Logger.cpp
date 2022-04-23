#include "pchheader.h"

#include "Logger.h"

namespace Horyzen {

	std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Logger::s_AppLogger;

	void Logger::Init()
	{
		const char format[] = "%^[%T] %n: %v%$";
		s_EngineLogger = spdlog::stdout_color_mt("Horyzen");
		s_EngineLogger->set_level(spdlog::level::trace);
		s_EngineLogger->set_pattern(format);

		s_AppLogger = spdlog::stdout_color_mt("Application");
		s_AppLogger->set_level(spdlog::level::trace);
		s_AppLogger->set_pattern(format);
	}

	std::shared_ptr<spdlog::logger>& Logger::GetEngineLogger()
	{
		return s_EngineLogger;
	}

	std::shared_ptr<spdlog::logger>& Logger::GetAppLogger()
	{
		return s_AppLogger;
	}

}