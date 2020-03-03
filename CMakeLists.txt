cmake_minimum_required(VERSION 3.0)

project(guitar-solo-part-generator C CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/src.wsjcpp/CMakeLists.txt)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${guitar-solo-part-generator_SOURCE_DIR})

# include header dirs
list (APPEND WSJCPP_INCLUDE_DIRS "src")
list (APPEND WSJCPP_INCLUDE_DIRS "src/server")

list (APPEND WSJCPP_SOURCES "src/main.cpp")
list (APPEND WSJCPP_SOURCES "src/guitar_solo_part_generator_enums.h")
list (APPEND WSJCPP_SOURCES "src/guitar_solo_part_generator_enums.cpp")
list (APPEND WSJCPP_SOURCES "src/guitar_solo_part_generate_filters.h")
list (APPEND WSJCPP_SOURCES "src/guitar_solo_part_generate_filters.cpp")
list (APPEND WSJCPP_SOURCES "src/position_note_guitar.h")
list (APPEND WSJCPP_SOURCES "src/position_note_guitar.cpp")
list (APPEND WSJCPP_SOURCES "src/solo_part_guitar_rule.h")
list (APPEND WSJCPP_SOURCES "src/solo_part_guitar_rule.cpp")
list (APPEND WSJCPP_SOURCES "src/solo_part_guitar_rules.h")
list (APPEND WSJCPP_SOURCES "src/solo_part_guitar_rules.cpp")
list (APPEND WSJCPP_SOURCES "src/solo_part_guitar.h")
list (APPEND WSJCPP_SOURCES "src/solo_part_guitar.cpp")
list (APPEND WSJCPP_SOURCES "src/server/http_handler_available_filters.h")
list (APPEND WSJCPP_SOURCES "src/server/http_handler_available_filters.cpp")
list (APPEND WSJCPP_SOURCES "src/server/http_handler_solo_generate.h")
list (APPEND WSJCPP_SOURCES "src/server/http_handler_solo_generate.cpp")

include_directories(${WSJCPP_INCLUDE_DIRS})

add_executable (guitar-solo-part-generator ${WSJCPP_SOURCES})

target_link_libraries(guitar-solo-part-generator ${WSJCPP_LIBRARIES})

install(
    TARGETS
        guitar-solo-part-generator
    RUNTIME DESTINATION
        /usr/bin
)

