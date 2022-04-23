#include "pchheader.h"
#include "Profiler.h"


namespace Horyzen {

	Profiler Profiler::s_Instance;

	struct ProfilingSession
	{
		std::string Name;
	};

	struct ProfileResultEntry
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	void Profiler::InitSession(const std::string& p_name, const std::string& p_filePath, bool p_enableImmediately)
	{
		m_OutputStream.open(p_filePath);

		// Writing header data
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[\n";
		m_OutputStream.flush();

		m_ProfilingSession = new ProfilingSession{ p_name };

		m_IsEnabled = p_enableImmediately;
	}

	void Profiler::StartProfiling()
	{
		m_IsEnabled = true;
	}

	void Profiler::StopProfiling()
	{
		m_IsEnabled = false;
	}

	void Profiler::EndSession()
	{
		// Writing a footer
		m_OutputStream << "]}";
		m_OutputStream.flush();

		m_OutputStream.close();

		delete m_ProfilingSession;
		m_ProfilingSession = nullptr;
		m_ProfileEntriesCount = 0;
		m_IsEnabled = false;
	}

	bool Profiler::IsEnabled() const
	{
		return m_IsEnabled;
	}

	std::string Profiler::GetSessionName() const
	{
		return m_ProfilingSession->Name;
	}

	void Profiler::WriteProfile(const ProfileResultEntry& result)
	{
		if (!m_IsEnabled) return;

		if (m_ProfileEntriesCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.Name;
		std::replace(name.begin(), name.end(), '"', '\'');

		m_OutputStream << "{\n";
		m_OutputStream << "   \"cat\":\"function\",\n";
		m_OutputStream << "   \"dur\":" << (result.End - result.Start) << ",\n";
		m_OutputStream << "   \"name\":\"" << name << "\",\n";
		m_OutputStream << "   \"ph\":\"X\",\n";
		m_OutputStream << "   \"pid\":0,\n";
		m_OutputStream << "   \"tid\":" << result.ThreadID << ",\n";
		m_OutputStream << "   \"ts\":" << result.Start << "\n";
		m_OutputStream << "}\n";

		m_OutputStream.flush();
	}

	Profiler& Profiler::GetProfiler()
	{
		return s_Instance;
	}




	ProfilerTimer::ProfilerTimer(const char* p_name)
		: m_Name(p_name)
	{
		m_StartTimePoint = std::chrono::high_resolution_clock::now();
	}

	ProfilerTimer::~ProfilerTimer()
	{
		if (!m_Stopped) {
			Stop();
		}
	}

	void ProfilerTimer::Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePoint).time_since_epoch().count();
		auto stop = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		ProfileResultEntry entry;
		entry.Name = m_Name;
		entry.Start = start;
		entry.End = stop;
		entry.ThreadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
		Profiler::GetProfiler().WriteProfile(entry);

		m_Stopped = true;
	}

}