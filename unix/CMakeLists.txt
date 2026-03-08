cmake_minimum_required(VERSION 3.26)
project(mini_git LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

add_executable(git_d
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/GitConfig.cpp
    ${SRC_DIR}/GitFolder.cpp
    ${SRC_DIR}/FileChunk.cpp
    ${SRC_DIR}/CryptoUtils.cpp
    ${SRC_DIR}/GitFile.cpp
    ${SRC_DIR}/ConfigDiskManager.cpp
)

target_compile_options(git_d PRIVATE
    $<$<CONFIG:Debug>:-g -O0>
    $<$<CONFIG:Release>:-O2>
)

if(APPLE)
    # CommonCrypto is part of the macOS system SDK — no extra linkage needed
elseif(UNIX)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(git_d PRIVATE OpenSSL::Crypto)
endif()
