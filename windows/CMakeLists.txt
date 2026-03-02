cmake_minimum_required(VERSION 3.26)
project(mini_git LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_executable(git_d
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/GitConfig.cpp
    ${SRC_DIR}/FileChunk.cpp
    ${SRC_DIR}/CryptoUtils.cpp
    ${SRC_DIR}/GitFile.cpp
)

target_link_libraries(git_d PRIVATE bcrypt)
