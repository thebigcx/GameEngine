project "GameEngine"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    buildoptions "-Wextra -Wnon-virtual-dtor -pedantic"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/Engine"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}/Engine"

    files {
        "src/**.cpp", "src/**.c",
        "vendor/stb_image/src/stb_image/stb_image.cpp",
        "vendor/dr_libs/src/dr_libs/dr_libs.c"
    }

    includedirs {
        "include",
        "include/vendor",
        "vendor/freetype2/include/freetype2",
        "vendor/dr_libs/include/",
        "vendor/glfw/include",
        "vendor/assimp/include",
        "vendor/maths",
        "vendor/stb_image/include"
    }

    optimize "On"
