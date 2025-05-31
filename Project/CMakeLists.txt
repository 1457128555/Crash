cmake_minimum_required(VERSION 3.22.1)

project(Crash)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(DEP)
add_subdirectory(Common)
add_subdirectory(Render)
add_subdirectory(Engine)
add_subdirectory(Client)