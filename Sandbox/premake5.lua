project "Sandbox"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/Sandbox"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}/Sandbox"

	files {
		"**.cpp",
		"**.h"
	}
	
	includedirs {
		"%{wks.location}/Engine/include",
        "%{wks.location}/Engine/include/vendor",
        "%{wks.location}/Engine/vendor/freetype2/include/freetype2",
        "%{wks.location}/Engine/vendor/dr_libs/include/",
        "%{wks.location}/Engine/vendor/glfw/include",
		"%{wks.location}/Engine/vendor/assimp/include",
        "%{wks.location}/Engine/vendor/maths",
        "%{wks.location}/Engine/vendor/stb_image/include"
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
		"freetype",
		"assimp"
	}
	
	optimize "On"
	
	configuration "Debug"
	configuration "Release"
