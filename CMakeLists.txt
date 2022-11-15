cmake_minimum_required(VERSION 3.10)

project(RayTracer12)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(${DIR}/lib/glm) # Folder where glm's CMakeLists.txt lives

add_executable(${PROJECT_NAME} 
	${DIR}/main.cpp
	${DIR}/src/out/ppm/PPMImageMeta.h
	${DIR}/src/out/ppm/PPMImageMeta.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_link_libraries(${PROJECT_NAME} PUBLIC glm) # Actually linking glm into the project

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR} ${DIR}/lib) # for resolving includes
