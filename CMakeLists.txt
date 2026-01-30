cmake_minimum_required(VERSION 3.26)
project(mini_git LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(git_d ./src/main.cpp ./src/GitConfig.cpp)
