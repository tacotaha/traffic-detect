cmake_minimum_required(VERSION 2.8)

project(traffic-detect)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -Wall -Werror")
message ("CMAKE_CXX_FLAGS is: ${CMAKE_CXX_FLAGS}")

find_package(OpenCV REQUIRED) 
include_directories(${OpenCV_INCLUDE_DIRS})
MESSAGE("OpenCV_LIBS: " ${OpenCV_LIBRARIES})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})# ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES}) 
