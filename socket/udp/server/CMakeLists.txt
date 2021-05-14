cmake_minimum_required (VERSION 3.2)
project(server)

find_package(Boost COMPONENTS program_options REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(server server.cpp)
target_link_libraries(server ${Boost_LIBRARIES})
