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
IncludeDir["Glad"] = "Dovah/vendor/Glad/include"
IncludeDir["ImGui"] = "Dovah/vendor/imgui"

include "Dovah/vendor/GLFW"
include "Dovah/vendor/Glad"
include "Dovah/vendor/imgui"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"		
		defines
		{
			"DOVAH_BUILD_DLL",
			"DOVAH_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}
		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Sandbox")
		}
	
	filter "configurations:Debug"
		defines "DOVAH_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "DOVAH_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "DOVAH_DIST"
		buildoptions "/MD"
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
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "DOVAH_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "DOVAH_DIST"
		buildoptions "/MD"
		optimize "On"
