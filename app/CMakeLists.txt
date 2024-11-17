cmake_minimum_required(VERSION 3.20)

set(HEADER_FILES
)

set(SOURCE_FILES
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} )

target_link_libraries(${PROJECT_NAME} sfml-graphics)
target_link_libraries(${PROJECT_NAME} sfml-audio)
