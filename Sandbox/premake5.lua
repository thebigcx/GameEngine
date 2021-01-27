project "Sandbox"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/Sandbox"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}/Sandbox"

	files {
		"src/SceneTesting/**.cpp",
		"src/SceneTesting/**.h"
	}
	--[[files {
		"src/TerrariaDemo/**.cpp",
		"src/TerrariaDemo/**.h"
	}]]--
	
	includedirs {
		"%{wks.location}/Engine/include",
        "%{wks.location}/Engine/include/vendor",
        "%{wks.location}/Engine/vendor/freetype2/include/freetype2",
        "%{wks.location}/Engine/vendor/dr_libs/include/",
        "%{wks.location}/Engine/vendor/glfw/include",
		"%{wks.location}/Engine/vendor/assimp/include",
        "%{wks.location}/Engine/vendor/maths",
		"%{wks.location}/Engine/vendor/stb_image/include",
		"%{wks.location}/Engine/vendor",
		"/usr/include/mono-2.0" -- TODO: fix this
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
		"assimp",
		"pthread",
		"yaml-cpp",
		"lua",
		"mono-2.0",
		"dl", -- TODO: move to static library
		"box2d"
	}
	
	filter "configurations:Debug"
		buildoptions "-pg"
        defines "ENGINE_DEBUG"
        runtime "Release"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
