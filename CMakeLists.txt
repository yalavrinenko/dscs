cmake_minimum_required(VERSION 3.17.5)

project(dscs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-fconcepts -Wall -Werror -Wno-sign-compare") #Fuck imgui

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

message(STATUS "[SRC] Found files: ${SRC} ${HEADERS}")

#IMGUI
find_package(SFML 2 REQUIRED COMPONENTS network graphics window system)
find_package(OpenGL)
find_package(ImGui-SFML REQUIRED)
find_package(X11)

set(GUI_SRC utils/guilogger.cpp utils/guilogger.hpp utils/gui_entries.cpp utils/gui_entries.hpp utils/gui_input.cpp utils/launcher_entry.cpp utils/launcher_entry.hpp utils/radar_entry.cpp utils/radar_entry.hpp)
add_library(gui SHARED ${GUI_SRC})
include_directories(~/Files/apis/SFML/build/include ~/Files/apis/imgui-sfml/build/include
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/src")

include(FetchContent)
FetchContent_Declare(
        imgui_plot
        GIT_REPOSITORY https://github.com/soulthreads/imgui-plot.git
        GIT_TAG v0.1.0
        EXCLUDE_FROM_ALL
)
FetchContent_GetProperties(imgui_plot)
if(NOT imgui_plot_POPULATED)
    FetchContent_Populate(imgui_plot)
    add_subdirectory(${imgui_plot_SOURCE_DIR} ${imgui_plot_BINARY_DIR})
endif()

target_link_libraries(gui sfml-graphics ${OPENGL_LIBRARIES} ImGui-SFML::ImGui-SFML pthread ${X11_LIBRARIES} imgui_plot)

add_library(logger utils/logger_factory.cpp utils/logger_factory.hpp)
target_link_libraries(logger stdc++fs)

add_subdirectory(tests)

add_library(dscs-lib ${SRC} ${HEADERS} src/ships/weapons/missiles/missile.cpp src/ships/weapons/missiles/missile.hpp src/ships/weapons/missiles/NAR_M.cpp src/ships/weapons/missiles/NAR_M.hpp src/ships/cargo/base_cargo.cpp src/ships/cargo/base_cargo.hpp src/ships/weapons/missiles/launcher.cpp src/ships/weapons/missiles/launcher.hpp src/ships/detection/object_tracker.cpp src/ships/detection/object_tracker.hpp)
add_executable(dscs entry.cpp)

target_link_libraries(dscs stdc++fs pthread gui dscs-lib logger)

add_executable(gui_test gui/imgui_test.cpp)
target_link_libraries(gui_test gui)



