cmake_minimum_required(VERSION 3.10)
# ---- Project settings ----
set(PROJECT_NAME HelloWorld)
project(${PROJECT_NAME} VERSION 1.0 LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Find GLib ----
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
if(NOT GLIB_FOUND)
    message(FATAL_ERROR "GLib not found. Please install libglib2.0-dev (e.g., sudo apt-get install libglib2.0-dev)")
endif()

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
      "In-source build is not allowed. \n"
      "Please create a separate build directory:\n"
      " mkdir build\n"
      " cd build\n"
      " cmake ..\n"
      " make")
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Debug build: enabling -g")
    add_compile_options(-g)
else()
    message(STATUS "Release build")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src")
    message(FATAL_ERROR "src directory not found!")
endif()
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/test")
    message(WARNING "test directory not found (tests will not be built)")
endif()
file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.c")
list(REMOVE_ITEM SRC_FILES "${CMAKE_SOURCE_DIR}/src/main.c")
add_library(${PROJECT_NAME}_lib ${SRC_FILES})
# ---- Link GLib to library ----
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${GLIB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_lib PUBLIC ${GLIB_LIBRARIES})
target_compile_options(${PROJECT_NAME}_lib PUBLIC ${GLIB_CFLAGS_OTHER})
add_executable(main src/main.c)
# ---- Link GLib to main executable ----
target_link_libraries(main PRIVATE ${PROJECT_NAME}_lib ${GLIB_LIBRARIES})
target_include_directories(main PUBLIC ${GLIB_INCLUDE_DIRS})
target_compile_options(main PUBLIC ${GLIB_CFLAGS_OTHER})
if(EXISTS "${CMAKE_SOURCE_DIR}/test")
    file(GLOB TEST_FILES "${CMAKE_SOURCE_DIR}/test/*.c")
    add_executable(tests ${TEST_FILES})
    # ---- Link GLib to tests ----
    target_link_libraries(tests PRIVATE ${PROJECT_NAME}_lib ${GLIB_LIBRARIES})
    target_include_directories(tests PUBLIC ${GLIB_INCLUDE_DIRS})
    target_compile_options(tests PUBLIC ${GLIB_CFLAGS_OTHER})
endif()
# ---- Clean target ----
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/CMakeCache.txt
    COMMENT "Removing all build files"
)
# ---- Run target ----
add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/main
    DEPENDS main
    WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    COMMENT "Running main program"
)
