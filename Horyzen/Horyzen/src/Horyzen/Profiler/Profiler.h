#pragma once
#include "pchheader.h"

#include "Horyzen/Core.h"

namespace Horyzen {

	struct ProfilingSession;
	struct ProfileResultEntry;
	class HORYZEN_OBJECT Profiler
	{
	public:

		void InitSession(const std::string& p_name, const std::string& p_filePath, bool p_enableImmediately);
		void StartProfiling();
		void StopProfiling();
		void EndSession();

		bool IsEnabled() const;
		std::string GetSessionName() const;

		void WriteProfile(const ProfileResultEntry& result);

		static Profiler& GetProfiler();

	private:

		ProfilingSession* m_ProfilingSession = nullptr;
		std::ofstream m_OutputStream;
		u32 m_ProfileEntriesCount = 0;

		static Profiler s_Instance;
		bool m_IsEnabled;

	};

	class HORYZEN_OBJECT ProfilerTimer
	{
	public:

		ProfilerTimer(const char* p_name);
		~ProfilerTimer();

		void Stop();

	private:

		std::chrono::time_point <std::chrono::high_resolution_clock> m_StartTimePoint;
		const char* m_Name;
		bool m_Stopped = false;

	};

}

// These defines help us to add line number to timer's name
#define CONCATENATE_INDIRECTION(x, y) x ## y
#define CONCATENATE(x, y) CONCATENATE_INDIRECTION(x ,y)

#define PROFILE_BUILD 1

#ifdef PROFILE_BUILD
	#define HORYZEN_INIT_PROFILE_SESSION(name) Horyzen::Profiler::GetProfiler().InitSession(name, name".json", false)
	#define HORYZEN_START_PROFILING()          Horyzen::Profiler::GetProfiler().StartProfiling()
	#define HORYZEN_STOP_PROFILING()           Horyzen::Profiler::GetProfiler().StopProfiling()
	#define HORYZEN_END_PROFILE_SESSION()      Horyzen::Profiler::GetProfiler().EndSession()

	#define HORYZEN_INIT_PROFILE_SESSION_AND_START(name) HORYZEN_INIT_PROFILE_SESSION(name); \
                                                         HORYZEN_START_PROFILING()

	#define HORYZEN_PROFILE_SCOPE(name) Horyzen::ProfilerTimer CONCATENATE(timer, __LINE__) (name)
	#define HORYZEN_PROFILE_FUNCTION()  HORYZEN_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define HORYZEN_INIT_PROFILE_SESSION
	#define HORYZEN_START_PROFILING
	#define HORYZEN_STOP_PROFILING
	#define HORYZEN_END_PROFILE_SESSION
	#define HORYZEN_INIT_PROFILE_SESSION_AND_START

	#define HORYZEN_PROFILE_SCOPE
	#define HORYZEN_PROFILE_FUNCTION
#endif
