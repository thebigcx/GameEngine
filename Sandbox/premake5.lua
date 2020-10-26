project "Sandbox"
	kind "ConsoleApp"
	language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/sandbox"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}/sandbox"
	files {
		"**.cpp",
		"**.h"
	}
	
	includedirs {
		"%{wks.location}/Engine/include",
		"%{wks.location}/Engine/include/vendor",
		"%{wks.location}/Engine/include/vendor/freetype2"
	}
	
	libdirs {
		"%{wks.location}/bin/Debug"
	}
	
	links {
		"GameEngine",
		"GL",
		"glfw",
		"GLEW",
		"openal",
		"freetype"
	}
	
	optimize "On"
	
	configuration "Debug"
	configuration "Release"