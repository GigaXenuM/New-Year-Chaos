cmake_minimum_required(VERSION 3.20)

project(New-Year-Chaos LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libexec)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BOX2D_BUILD_TESTBED "BOX2D: Build the testbed" OFF)
option(BOX2D_BUILD_UNIT_TESTS "BOX2D: Build the unit tests" OFF)
option(EARCUT_BUILD_VIZ "EARCUT: Build the earcut visualizer program" OFF)

add_subdirectory(libs/sfml)
add_subdirectory(libs/box2d)
add_subdirectory(libs/earcut)
add_subdirectory(libs/3rdparty/tmxlite)
add_subdirectory(app)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

file(COPY ${CMAKE_SOURCE_DIR}/app/res/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
