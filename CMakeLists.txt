cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(SRC src/void_map.cpp src/void_map.hpp
        src/void_object.cpp src/void_object.hpp
        src/common.cpp src/common.hpp
        src/interface.hpp
        src/void_display.cpp
        src/void_display.hpp
        src/drifting_hulk.hpp
        src/ships/small.cpp
        src/ships/small.hpp
        src/ships/hull.hpp
        src/ships/component.hpp
        src/ships/tank/battery.hpp
        src/ships/power/reactor.hpp
        src/ships/tank/iaccumalative_container.hpp
        src/ships/tank/icontainer_adapter.cpp
        src/ships/tank/icontainer_adapter.hpp
        src/ships/tank/pipe_and_wire.cpp
        src/ships/tank/pipe_and_wire.hpp
        src/ships/power/reactor.cpp src/ships/engines/engine.cpp src/ships/engines/engine.hpp src/component-factrory.cpp src/component-factrory.hpp)

add_library(logger src/logger_factory.cpp src/logger_factory.hpp)
target_link_libraries(logger stdc++fs)

add_subdirectory(tests)

add_library(dscs-lib ${SRC})
add_executable(dscs entry.cpp)

target_link_libraries(dscs stdc++fs pthread logger dscs-lib)