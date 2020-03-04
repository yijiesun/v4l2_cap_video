
cmake_minimum_required (VERSION 2.8)
project(VIDEO_CAP)
add_definitions(-std=c++11)

set(CMAKE_BUILD_TYPE "Release")

set( VIDEO_CAP_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/v4l2/v4l2.h
${CMAKE_CURRENT_SOURCE_DIR}/src/v4l2/v4l2.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/screen/screen.h
${CMAKE_CURRENT_SOURCE_DIR}/src/screen/screen.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/menu_config/menu_config.h
${CMAKE_CURRENT_SOURCE_DIR}/src/menu_config/menu_config.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/funs/funs.h
${CMAKE_CURRENT_SOURCE_DIR}/src/funs/funs.cpp
   )
#opencv
find_package(OpenCV REQUIRED)

#exe
add_executable(video_cap ${VIDEO_CAP_SRCS})
target_link_libraries(video_cap ${OpenCV_LIBS}  pthread)



