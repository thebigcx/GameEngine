project "GameEngine"

    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    --buildoptions "-Wall -Wextra -Wnon-virtual-dtor -pedantic"
    --buildoptions "-Wuninitialized"

    targetdir "%{wks.location}/bin/%{cfg.buildcfg}/Engine"
    objdir "%{wks.location}/obj/%{cfg.buildcfg}/Engine"

    files {
        "src/**.cpp", "src/**.c",
        "vendor/imgui/*.cpp",
        "vendor/imgui/backends/imgui_impl_glfw.cpp",
        "vendor/imgui/backends/imgui_impl_opengl3.cpp",
        "vendor/stb_image/src/stb_image/stb_image.cpp",
        "vendor/dr_libs/src/dr_libs/dr_libs.c",
        "vendor/imguizmo/*.cpp",
        "vendor/yaml/include"
    }

    includedirs {
        "include",
        "include/vendor",
        "vendor/freetype2/include/freetype2",
        "vendor/dr_libs/include/",
        "vendor/glfw/include",
        "vendor/assimp/include",
        "vendor/maths",
        "vendor/stb_image/include",
        "vendor/imgui",
        "vendor",
        "vendor/box2d/include",
        "vendor/Mono/include",
        "/usr/include/mono-2.0" -- TODO: fix this
    }

    filter "configurations:Debug"
        buildoptions "-pg"
        defines "ENGINE_DEBUG"
        runtime "Release"
        --optimize "On" -- TODO: not optimize in Debug mode
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
