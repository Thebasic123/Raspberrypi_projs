cmake_minimum_required (VERSION 3.2)

project(video_streaming)

find_package(Boost COMPONENTS program_options REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${EIGEN_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Default to Release mode.
if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(${CMAKE_BUILD_TYPE} Release ... FORCE)
endif()

# Release mode makes the code run faster.
if(NOT ${CMAKE_BUILD_TYPE} MATCHES Debug)
  message("Build type: Release.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -Ofast")
else()
  message("WARNING: Building in DEBUG MODE! Use 'cmake -DCMAKE_BUILD_TYPE=Release' for faster runtime.")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Libraries will be stored in the "lib" directory, and binaries in "bin".
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Grab all binary source files in the src directory.
file(GLOB SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*")


# Create a library for all of the added Super Resolution dependencies (i.e. all
# the classes).
file(GLOB   protocols_SRC   "src/protocols/*.cpp")
file(GLOB   receiver_SRC    "src/receiver/*.cpp")
file(GLOB   sender_SRC      "src/sender/*.cpp")
file(GLOB   video_SRC       "src/video/*.cpp")

# Add the receiver binary.
add_executable(
  receiver
  src/receiver.cpp
  ${protocols_SRC}
  ${receiver_SRC}
  ${video_SRC}
)
target_link_libraries(
  receiver
  ${OpenCV_LIBS}
  ${Boost_LIBRARIES}
)

# Add the sender binary.
add_executable(
  sender
  src/sender.cpp
  ${protocols_SRC}
  ${sender_SRC}
  ${video_SRC}
)
target_link_libraries(
  sender
  ${OpenCV_LIBS}
  ${Boost_LIBRARIES}
)
