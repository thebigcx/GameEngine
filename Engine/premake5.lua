project "GameEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/Engine"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}/Engine"

    buildoptions "-g"

    files {
        "src/**.cpp", "src/**.c",
        "include/**.h", "include/**.hpp"
    }

    includedirs {
        "include",
        "include/vendor",
        "include/vendor/freetype2"
    }

    links {
        "GL",
        "glfw",
        "GLEW",
        "openal",
        "freetype"
    }

    optimize "On"