 
cmake_minimum_required (VERSION 2.8)
project(vSLAM_FrontEnd)
#Version Numbers
set(CMAKE_VERBOSE_MAKEFILE TRUE)



include(${CMAKE_SOURCE_DIR}/cmake/StructuresConfig.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/OpenCVConfig.cmake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE FILEPATH  "Build binary directory")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE FILEPATH "build static libs directory")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib CACHE FILEPATH "Build Dynamic Libs directory")

list(APPEND CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake})




find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(
  include
  /usr/local/include
  /usr/local
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${Structures_INCLUDE_DIRS}

)

link_directories(${Structures_LIBRARIES_DIR})
link_directories(/usr/local/kaze/lib)
add_subdirectory(src)
