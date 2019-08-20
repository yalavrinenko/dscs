cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

set(SRC entry.cpp src/void_map.cpp src/void_map.hpp
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
        src/ships/tank/iaccumalative_container.hpp src/ships/tank/icontainer_adapter.cpp src/ships/tank/icontainer_adapter.hpp src/logger_factory.cpp src/logger_factory.hpp)

add_executable(dscs ${SRC})

target_link_libraries(dscs stdc++fs pthread)