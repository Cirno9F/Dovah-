workspace "Dovah"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Dovah/vendor/GLFW/include"
IncludeDir["Glad"] = "Dovah/vendor/Glad/include"
IncludeDir["ImGui"] = "Dovah/vendor/imgui"
IncludeDir["glm"] = "Dovah/vendor/glm"
IncludeDir["stb_image"] = "Dovah/vendor/stb_image"

include "Dovah/vendor/GLFW"
include "Dovah/vendor/Glad"
include "Dovah/vendor/imgui"

--group "Dependencies"
	--include "Dovah/vendor/GLFW"
	--include "Dovah/vendor/Glad"
	--include "Dovah/vendor/imgui"
--group ""


project "Dovah"
	location "Dovah"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dovahpch.h"
	pchsource "Dovah/src/dovahpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	    "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/glm/glm/**.hpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		systemversion "latest"		
		defines
		{
			"DOVAH_BUILD_DLL",
			"DOVAH_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
		}
	
	filter "configurations:Debug"
		defines "DOVAH_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DOVAH_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DOVAH_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Dovah/vendor/spdlog/include",
		"Dovah/src",
		"Dovah/vendor",
		"%{IncludeDir.glm}",
	}

	links
	{
		"Dovah",
	}

	filter "system:windows"
		systemversion "latest"		
		defines
		{
			"DOVAH_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DOVAH_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "DOVAH_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "DOVAH_DIST"
		runtime "Release"
		optimize "on"
