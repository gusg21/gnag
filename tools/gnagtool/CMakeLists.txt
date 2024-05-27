cmake_minimum_required(VERSION 3.21)
project(gnagtool LANGUAGES CXX C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(SDL_STATIC ON)
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

if (WIN32)
    set(WIN32_EXTRAS)
else ()
    set(WIN32_EXTRAS
            source/mouse.cpp
            source/mouse.h
    )
endif ()
add_executable(gnagtool_exe ${WIN32_EXTRAS}
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

)
target_include_directories(gnagtool_exe PUBLIC imgui)

target_link_libraries(gnagtool_exe PUBLIC Shlwapi)
target_link_libraries(gnagtool_exe PUBLIC SDL2::SDL2)
if (WIN32)
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
target_link_libraries(gnagtool_exe PUBLIC SDL2_image::SDL2_image)


# Copy Gnag art to output
add_custom_command(TARGET gnagtool_exe POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "$ENV{GNAG_PATH}/gfx/"
        $<TARGET_FILE_DIR:gnagtool_exe>/gfx/)