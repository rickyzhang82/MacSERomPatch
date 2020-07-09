cmake_minimum_required(VERSION 3.5)

project(MacSERomPatch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MacSERomPatch main.cpp common.cpp)
add_executable(utils utils.cpp common.cpp)
