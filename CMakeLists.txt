cmake_minimum_required(VERSION 3.10)

project(RayTracer12)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(RayTracer12 
	${DIR}/main.cpp
	${DIR}/src/out/ppm/PPMImage.h
	${DIR}/src/out/ppm/PPMImage.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)