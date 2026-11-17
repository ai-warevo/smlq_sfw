cmake_minimum_required(VERSION 3.15)
project(smlq_sfw LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Td REQUIRED)

include_directories(include)

add_subdirectory(src)
