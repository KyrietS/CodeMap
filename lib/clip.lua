project "clip"
    location ""
    language "C++"
    cppdialect "C++17"
    targetname "clip"
    kind "StaticLib"
    warnings "Off"

    filter "configurations:Debug"
        symbols "On"

    filter "action:vs*"
        defines{"_SCL_SECURE_NO_WARNINGS"}
        
    filter{}
    vpaths {
        ["Header Files"] = { "clip/**.h"},
        ["Source Files/*"] = {"clip/**.c"},
    }
    files {
        "clip/clip.cpp",
        "clip/image.cpp"
    }
    files {
        "clip/clip.h",
        "clip/clip_common.h",
        "clip/clip_lock_impl.h"
    }

    filter "system:Windows"
        files { "clip/clip_win.cpp", "clip/clip_win_wic.h" }
        links { "shlwapi.lib" }
