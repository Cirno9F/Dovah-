workspace "Dovah"
	architecture "x64"

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

include "Dovah/vendor/GLFW"

project "Dovah"
	location "Dovah"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dovahpch.h"
	pchsource "Dovah/src/dovahpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"		
		defines
		{
			"DOVAH_BUILD_DLL",
			"DOVAH_PLATFORM_WINDOWS"
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Sandbox")
		}
	
	filter "configurations:Debug"
		defines "DOVAH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "DOVAH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "DOVAH_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

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
		"Dovah/src"
	}

	links
	{
		"Dovah"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"		
		defines
		{
			"DOVAH_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DOVAH_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "DOVAH_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "DOVAH_DIST"
		optimize "On"
