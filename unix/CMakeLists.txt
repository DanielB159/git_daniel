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

if(APPLE)
    # CommonCrypto is part of the macOS system SDK — no extra linkage needed
elseif(UNIX)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(git_d PRIVATE OpenSSL::Crypto)
endif()
