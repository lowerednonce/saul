cmake_minimum_required(VERSION 3.21)
project(saul-demo VERSION 1.1)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")

set(sources
    src/main.cpp
)

add_subdirectory(lib)
add_executable(saul-demo src/main.cpp)
target_link_libraries(saul-demo
    PUBLIC
        saul
    PRIVATE
        SDL2
        SDL2_ttf
        SDL2_image
)
