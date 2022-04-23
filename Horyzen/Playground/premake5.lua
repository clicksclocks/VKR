-- Playground premake5 file
project "Playground"
	kind "ConsoleApp"
	language "C++"
	targetdir "../bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir    "../obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"../Horyzen/src",
		"../Horyzen/deps/spdlog/include",
		"../Horyzen/deps"
	}

	links
	{
		"Horyzen"
	}

	filter {"configurations:Debug"}
		defines
		{
			"HORYZEN_DEBUG_BUILD",
			"HORYZEN_ENABLE_ASSERTIONS"
		}
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