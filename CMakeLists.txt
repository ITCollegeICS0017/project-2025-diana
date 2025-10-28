cmake_minimum_required(VERSION 3.15)
project(railway_ticket_sales LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Make header path visible (global)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Collect all source files under src/
file(GLOB_RECURSE APP_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Single target that builds the whole app (main.cpp must be present exactly once)
add_executable(app ${APP_SOURCES})

# Make include dir target-local as well (optional)
target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optional: build tests if tests/tests.cpp exists (create a library from all sources except main.cpp)
if(EXISTS "${CMAKE_SOURCE_DIR}/tests/tests.cpp")
    set(LIB_SOURCES "")
    foreach(f ${APP_SOURCES})
        if(NOT f MATCHES "/src/main.cpp$")
            list(APPEND LIB_SOURCES ${f})
        endif()
    endforeach()

    add_library(project_lib STATIC ${LIB_SOURCES})
    target_include_directories(project_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

    add_executable(ticket_tests ${CMAKE_SOURCE_DIR}/tests/tests.cpp)
    target_link_libraries(ticket_tests PRIVATE project_lib)
    set_target_properties(ticket_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

# Make output directory predictable
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
