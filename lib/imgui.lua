project "imgui"
    location ""
    language "C++"
    targetname "imgui"
    kind "StaticLib"
    warnings "Off"

    filter "configurations:Debug OR Release"

    filter "configurations:Debug"
        symbols "On"
        
    filter{}
	
    vpaths {
        ["Header Files"] = { "imgui/**.h"},
        ["Source Files/*"] = {"imgui/**.cpp"},
    }
    files {
        "imgui/*.h", 
        "imgui/*.cpp"
    }
