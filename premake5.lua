workspace "CodeMap"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	startproject "CodeMap"

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"	

	filter {}

	targetdir "build/bin/%{cfg.buildcfg}"
	objdir "build/obj/%{cfg.buildcfg}/%{prj.name}"

	group "Dependencies"
		include "lib/raylib.lua"
        include "lib/clip.lua"
	group ""

	project "CodeMap"
		kind "ConsoleApp"
		location ""
		language "C++"
		cppdialect "C++17"

		includedirs {
			"lib/raylib/src",
            "lib/clip/"
		}
		files {"src/**.cpp", "src/**.hpp"}

		vpaths { ["*"] = "src" }
		
		links {"raylib", "clip"}
		dependson {"raylib", "clip"}
		
		libdirs {"build/bin/%{cfg.buildcfg}"}
		
		filter "action:vs*"
			defines{"_WINSOCK_DEPRECATED_NO_WARNINGS", "_CRT_SECURE_NO_WARNINGS", "_WIN32"}
			links 
			{
				"raylib.lib", "opengl32.lib", "kernel32.lib", 
				"user32.lib", "gdi32.lib", "winmm.lib", 
				"winspool.lib", "comdlg32.lib", "advapi32.lib", 
				"shell32.lib", "ole32.lib", "oleaut32.lib", 
				"uuid.lib", "odbc32.lib", "odbccp32.lib"
			}

		filter "action:gmake*"
			links {"pthread", "GL", "m", "dl", "rt", "X11"}


newaction {
    trigger     = "clean",
    description = "Clean project from binaries and obj files",
    execute = function ()
        print("Removing build/...")
        os.rmdir("build")
        print("Removing Visual Studio files...")
        os.rmdir(".vs")
        os.remove("*.sln")
        os.remove("lib/*.vcxproj*")
        os.remove("*.vcxproj*")
        print("Done")
    end
}