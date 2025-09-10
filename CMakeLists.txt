cmake_minimum_required(VERSION 3.21)
project(sandstone VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(pugixml CONFIG REQUIRED)

add_executable(sandstone src/main.cpp
        src/core/simulation.cpp
        src/core/simulation.h
        src/types/vector2i.cpp
        src/types/vector2i.h
        src/elements/element_type.cpp
        src/elements/element_type.h
        src/core/abstract/base_loader.h
        src/core/abstract/base_registry.h
        src/elements/element_registry.cpp
        src/elements/element_registry.h
        src/elements/element_loader.cpp
        src/elements/element_loader.h
        src/elements/types/abstract/solid.cpp
        src/elements/types/abstract/solid.h
        src/elements/types/movable_solid.cpp
        src/elements/types/movable_solid.h
        src/elements/types/liquid.cpp
        src/elements/types/liquid.h
        src/elements/types/immovable_solid.cpp
        src/elements/types/immovable_solid.h
        src/elements/types/abstract/solid.cpp
        src/elements/types/abstract/solid.h
        src/core/cell_data.cpp
        src/core/cell_data.h
        src/core/cell_matrix.cpp
        src/core/cell_matrix.h
        src/elements/empty.cpp
        src/elements/empty.h
        src/systems/input_system.cpp
        src/systems/input_system.h
        src/elements/types/gas.cpp
        src/elements/types/gas.h
        src/utils/element_type_checker.cpp
        src/utils/element_type_checker.h
        src/utils/movement_utils.cpp
        src/utils/movement_utils.h
        src/utils/random_utils.cpp
        src/utils/random_utils.h)
target_link_libraries(sandstone PRIVATE raylib glfw pugixml::pugixml)

# Copy data directory to build dir
add_custom_command(TARGET sandstone POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:sandstone>/data/elements
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/data
            $<TARGET_FILE_DIR:sandstone>/data)

if(UNIX AND NOT APPLE)
    target_link_libraries(sandstone PRIVATE m pthread dl rt X11)
endif()
