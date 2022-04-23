-- Main premake5 file
workspace "Horyzen"
	architecture "x86_64"
	configurations { "Debug", "Release", "Distribution" }
	startproject "Playground"

include "Horyzen/deps/glfw_premake5.lua"
include "Horyzen/deps/Glad_premake5.lua"
include "Horyzen/deps/ImGui_premake5.lua"
include "Horyzen/deps/ImGuiNodeEditor_premake5.lua"

include "Horyzen"
include "Playground"