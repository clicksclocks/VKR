-- Horyzen premake5 file
project "Horyzen"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
	targetdir "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir    "../obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	pchheader "pchheader.h"
	pchsource "src/pchsource.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"deps/spdlog/include",
		"deps/glfw/include",
		"deps/Glad/include",
		"deps/ImGui",
		"deps/ImGuiNodeEditor"
	}

	defines
	{
		"HORYZEN_BUILD"
	}

	links
	{
		"glfw",
		"Glad",
		"ImGui",
		"ImGuiNodeEditor"
	}

	filter {"configurations:Debug"}
		defines { "HORYZEN_DEBUG_BUILD", "HORYZEN_ENABLE_ASSERTIONS" }
		symbols "On"

	filter {"configurations:Release"}
		defines "HORYZEN_RELEASE_BUILD"
		optimize "On"

	filter {"configurations:Distribution"}
		defines "HORYZEN_DISTRIBUTION_BUILD"
		optimize "On"

	filter {"system:windows"}
		defines "HORYZEN_WINDOWS_BUILD"
		cppdialect "C++17"
		systemversion "latest"
		buildoptions "/Wall"

		postbuildcommands
		{
			"copy ..\\bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\Horyzen\\Horyzen.dll ..\\bin\\%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}\\Playground\\Horyzen.dll"
		}