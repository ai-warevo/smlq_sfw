cmake_minimum_required(VERSION 3.15)
project(tg_userbot CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Td REQUIRED)

add_subdirectory(src)
