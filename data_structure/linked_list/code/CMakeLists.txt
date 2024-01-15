cmake_minimum_required(VERSION 3.16)
project(Linked_list)

set(CMAKE_CXX_STANDARD 17) 


set(BUILD_DIR build)

file(GLOB SRC src/*.c )


add_executable(${PROJECT_NAME} ${SRC})



