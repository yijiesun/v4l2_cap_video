
cmake_minimum_required (VERSION 2.8)
project(SCREEN_SHOW)
add_definitions(-std=c++11)

set( SCREEN_SHOW_SRCS
${CMAKE_CURRENT_SOURCE_DIR}/src/screen_show.cpp
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
add_executable(SCREEN_SHOW ${SCREEN_SHOW_SRCS})
target_link_libraries(SCREEN_SHOW ${OpenCV_LIBS}  pthread)



