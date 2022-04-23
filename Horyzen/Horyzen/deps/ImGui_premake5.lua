-- ImGui premake5 file
project "ImGui"
	location "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir "%{prj.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir    "%{prj.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	files
	{
		"%{prj.location}/imconfig.h",
		"%{prj.location}/imgui.cpp",
		"%{prj.location}/imgui.h",
		"%{prj.location}/imgui_draw.cpp",
		"%{prj.location}/imgui_internal.h",
		"%{prj.location}/imgui_tables.cpp",
		"%{prj.location}/imgui_widgets.cpp",
		"%{prj.location}/imstb_rectpack.h",
		"%{prj.location}/imstb_textpack.h",
		"%{prj.location}/imstb_truetype.h",
		"%{prj.location}/imgui_demo.cpp"
	}

	defines
	{
		"IMGUI_API=__declspec(dllexport)"
	}


	filter {"system:windows"}
		systemversion "latest"
	

	filter {"system:linux"}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"