project "raylib"
    location ""
    language "C"
    targetname "raylib"
    kind "StaticLib"
    warnings "Off"

    defines {"PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33", "SUPPORT_CUSTOM_FRAME_CONTROL"}

    filter "configurations:Debug OR Release"

    filter "configurations:Debug"
        symbols "On"

    filter "action:vs*"
        defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
        
    filter{}

    includedirs { "raylib/src", "raylib/src/external/glfw/include"}
    vpaths {
        ["Header Files"] = { "raylib/src/**.h"},
        ["Source Files/*"] = {"raylib/src/**.c"},
    }
    files {"raylib/src/*.h", "raylib/src/*.c"}
