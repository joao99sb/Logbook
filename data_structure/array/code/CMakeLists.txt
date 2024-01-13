cmake_minimum_required(VERSION 3.16)
project(Array)

set(CMAKE_CXX_STANDARD 17) 

# Dependencies
include(FetchContent)



set(BUILD_DIR build)


file(GLOB SRC src/*.c )


add_executable(${PROJECT_NAME} ${SRC})


