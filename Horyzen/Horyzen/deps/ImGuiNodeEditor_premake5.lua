-- ImGuiNodeEditor premake5 file
project "ImGuiNodeEditor"
	location "ImGuiNodeEditor"
	kind "StaticLib"
	language "C++"

	targetdir "%{prj.location}/bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"
	objdir    "%{prj.location}/obj/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/%{prj.name}"

	files
	{
		"%{prj.location}/crude_json.cpp",
		"%{prj.location}/crude_json.h",
		"%{prj.location}/imgui_bezier_math.h",
		"%{prj.location}/imgui_canvas.cpp",
		"%{prj.location}/imgui_canvas.h",
		"%{prj.location}/imgui_extra_math.h",
		"%{prj.location}/imgui_node_editor.cpp",
		"%{prj.location}/imgui_node_editor.h",
		"%{prj.location}/imgui_node_editor_api.cpp",
		"%{prj.location}/imgui_node_editor_internal.h"
	}
	
	includedirs
	{
		"ImGui"
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