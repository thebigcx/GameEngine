workspace "Fast_Game_Library"
	configurations {"Debug", "Release"}

project "FGL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	targetdir "bin/%{cfg.buildcfg}"
	objdir "obj/%{cfg.buildcfg}"
	
	files {
		"FGL/src/**.cpp", "FGL/src/**.c",
		"FGL/include/**.h", "FGL/include/**.hpp"
	}
	
	includedirs {
		"FGL/include",
		"FGL/include/vendor",
		"FGL/include/vendor/freetype2"
	}
	
	links {
		"GL",
		"glfw",
		"GLEW",
		"openal",
		"freetype"
	}
	
	optimize "On"

project "Test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	files {
		"sandbox/**.cpp",
		"sandbox/**.h"
	}
	
	includedirs {
		"FGL/include",
		"FGL/include/vendor",
		"FGL/include/vendor/freetype2"
	}
	
	libdirs {
		"bin/Debug"
	}
	
	links {
		"FGL",
		"GL",
		"glfw",
		"GLEW",
		"openal",
		"freetype"
	}
	
	optimize "On"
	
	configuration "Debug"
	configuration "Release"


	
	
	
	

