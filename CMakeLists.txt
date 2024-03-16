cmake_minimum_required(VERSION 3.16)
include(FetchContent)

FetchContent_Declare(
    clip
    GIT_REPOSITORY https://github.com/dacap/clip.git
    GIT_TAG        v1.5
)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        0.9.9.8
)
FetchContent_Declare(
    reasings
    GIT_REPOSITORY https://github.com/raylib-extras/reasings.git
    GIT_TAG        b93d752bc7a23dcd27686bd2cdb2d07d3634455b
)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/KyrietS/raylib.git
    GIT_TAG        windows-compat
)
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.11.0
)
FetchContent_Declare(
    utf8cpp
    GIT_REPOSITORY https://github.com/nemtrif/utfcpp.git
    GIT_TAG        v3.2.3
)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        docking
)
FetchContent_Declare(
    tinyxml2
    GIT_REPOSITORY https://github.com/leethomason/tinyxml2.git
    GIT_TAG        9.0.0
)
FetchContent_Declare(
    EnTT
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.11.1
)
FetchContent_Declare(
    trex
    GIT_REPOSITORY https://github.com/KyrietS/trex.git
    GIT_TAG        00fb684f292f3b2ff2f3e236fc3404091fe6b392
)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        5736b15f7ea0ffb08dd38af21067c314d6a3aae9
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CodeMap)

project(CodeMap)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES "src/**.cpp" "src/**.hpp")
add_executable(CodeMap ${SOURCES})
target_include_directories(CodeMap PRIVATE src)
target_link_libraries(CodeMap raylib clip imgui tinyxml2 spdlog glm utf8cpp EnTT trex)
target_precompile_headers(CodeMap PRIVATE src/pch.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src FILES ${SOURCES})

set(CLIP_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CLIP_TESTS OFF CACHE BOOL "" FORCE)
set(tinyxml2_BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(
    clip
    glm
    reasings
    raylib
    spdlog
    utf8cpp
    imgui
    tinyxml2
    EnTT
    trex
    stb
)

target_compile_definitions(raylib PRIVATE SUPPORT_CUSTOM_FRAME_CONTROL)
target_compile_definitions(CodeMap PRIVATE "$<$<CONFIG:DEBUG>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>")
target_include_directories(CodeMap PRIVATE ${reasings_SOURCE_DIR}/src)
target_include_directories(CodeMap PRIVATE ${clip_SOURCE_DIR})
target_include_directories(CodeMap PRIVATE ${imgui_SOURCE_DIR})
target_include_directories(CodeMap PRIVATE ${stb_SOURCE_DIR})

# Use Unicode version of the Win32 API
if(WIN32)
    target_compile_definitions(CodeMap PRIVATE UNICODE _UNICODE)
endif()

file(GLOB IMGUI_SOURCES ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h ${imgui_SOURCE_DIR}/misc/cpp/*)
add_library(imgui ${IMGUI_SOURCES})
target_include_directories(imgui PRIVATE ${imgui_SOURCE_DIR})

get_target_property(dependencies CodeMap INTERFACE_LINK_LIBRARIES)
foreach(dependency ${dependencies})
    set_target_properties(${dependency} PROPERTIES FOLDER "Dependencies")
endforeach()

# Workaround for broken CLion's default MinGW toolset.
# See my bug report: https://youtrack.jetbrains.com/issue/CPP-33763/
if(WIN32 AND MINGW)
    # Needed by Clip library
    target_link_libraries(CodeMap windowscodecs)
endif()