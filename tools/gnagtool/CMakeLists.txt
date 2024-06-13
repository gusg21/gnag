cmake_minimum_required(VERSION 3.21)
project(gnagtool LANGUAGES CXX C)

if (NOT DEFINED ENV{GNAG_PATH})
    message(FATAL_ERROR "GNAG_PATH Environment variable not set!")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")


set(SDL_STATIC ON)
set(BUILD_SHARED_LIBS OFF)
add_subdirectory(external/SDL)
# Who needs like any of these lmao
set(SDL2IMAGE_AVIF OFF)
set(SDL2IMAGE_BMP OFF)
set(SDL2IMAGE_GIF OFF)
set(SDL2IMAGE_JPG OFF)
set(SDL2IMAGE_JXL OFF)
set(SDL2IMAGE_LBM OFF)
set(SDL2IMAGE_PCX OFF)
set(SDL2IMAGE_PNG ON)
set(SDL2IMAGE_PNM OFF)
set(SDL2IMAGE_QOI OFF)
set(SDL2IMAGE_SVG OFF)
set(SDL2IMAGE_TGA OFF)
set(SDL2IMAGE_TIF OFF)
set(SDL2IMAGE_WEBP OFF)
set(SDL2IMAGE_XCF OFF)
set(SDL2IMAGE_XPM OFF)
set(SDL2IMAGE_TGA OFF)
set(SDL2IMAGE_XV OFF)
add_subdirectory(external/SDL_image)
add_subdirectory(external/SDL_ttf)

string(REPLACE "\\" "/" GNAG_PATH_SAFE $ENV{GNAG_PATH})
FILE(GLOB gnagcommon_src ${GNAG_PATH_SAFE}/common/*.c)
set(gnagcommon_inc ${GNAG_PATH_SAFE}/common/)
message(STATUS "GnagCommon INC: ${gnagcommon_inc}")
message(STATUS "GnagCommon SRC: ${gnagcommon_src}")

add_executable(gnagtool_exe ${WIN32_EXTRAS}
        ${gnagcommon_src}

        imgui/imgui.cpp
        imgui/imgui.h
        imgui/imconfig.h
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_internal.h
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/imstb_rectpack.h
        imgui/imstb_textedit.h
        imgui/imstb_truetype.h
        imgui/misc/cpp/imgui_stdlib.cpp

        imgui/backends/imgui_impl_sdlrenderer2.cpp
        imgui/backends/imgui_impl_sdlrenderer2.h
        imgui/backends/imgui_impl_sdl2.cpp
        imgui/backends/imgui_impl_sdl2.h

        source/sdlt3s.cpp

        source/main.cpp
        source/fileexplorer.cpp
        source/fileexplorer.h
        source/gnagoswrapper.h
        source/gnagoswrapper.cpp
        source/scenarioeditor.cpp
        source/scenarioeditor.h
        source/gnagtool.cpp
        source/gnagtool.h
        source/toolgui.cpp
        source/scenario.cpp
        source/scenario.h
        source/mouse.cpp
        source/cJSON.c
        source/uilayouteditor.cpp
        source/uilayouteditor.h
        source/jsonserializable.cpp
        source/jsonserializable.h
        source/sdlt3s.cpp
        source/sdlt3s.h

)
target_include_directories(gnagtool_exe PUBLIC ${gnagcommon_inc})
target_include_directories(gnagtool_exe PUBLIC imgui)

target_link_libraries(gnagtool_exe PUBLIC SDL2::SDL2-static)
if (WIN32)
    target_link_libraries(gnagtool_exe PUBLIC Shlwapi)
    target_link_libraries(gnagtool_exe PUBLIC SDL2::SDL2main)
#    # Copies required DLLs to output :)
#    add_custom_command(TARGET gnagtool_exe POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${PROJECT_BINARY_DIR}/external/SDL/SDL2d.dll"
#            $<TARGET_FILE_DIR:gnagtool_exe>)
#    add_custom_command(TARGET gnagtool_exe POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            "${PROJECT_BINARY_DIR}/external/SDL_image/SDL2_imaged.dll"
#            $<TARGET_FILE_DIR:gnagtool_exe>)
endif ()
target_link_libraries(gnagtool_exe PUBLIC SDL2_image::SDL2_image-static)
target_link_libraries(gnagtool_exe PUBLIC SDL2_ttf::SDL2_ttf-static)

# Make sure to put the runtime in the build/ folder, not a build config-related subfolder
if ( MSVC )
    set_target_properties( gnagtool_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set_target_properties( gnagtool_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    set_target_properties( gnagtool_exe PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
    # etc for the other available configuration types (MinSizeRel, RelWithDebInfo)
endif ( MSVC )

# Copy Gnag art to output (/gfx/)
add_custom_command(TARGET gnagtool_exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$ENV{GNAG_PATH}/gfx/"
        $<TARGET_FILE_DIR:gnagtool_exe>/gfx/)

# Copy GnagTool assets to output (/assets/)
add_custom_command(TARGET gnagtool_exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets/"
        $<TARGET_FILE_DIR:gnagtool_exe>/assets/)

set_target_properties(
    gnagtool_exe PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

