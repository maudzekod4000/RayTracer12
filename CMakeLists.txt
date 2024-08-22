cmake_minimum_required(VERSION 3.10)

project(RayTracer12)

set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

include("${CMAKE_SOURCE_DIR}/lib/rapidjson/rapidjson.cmake")
add_subdirectory(${DIR}/lib/glm) # Folder where glm's CMakeLists.txt lives

add_executable(${PROJECT_NAME} 
	${DIR}/main.cpp
	${DIR}/src/out/ppm/PPMImageMeta.h
	${DIR}/src/out/ppm/PPMImageMeta.cpp
	${DIR}/src/out/ppm/PPMImage.h
	${DIR}/src/out/ppm/PPMImage.cpp
	${DIR}/src/out/ppm/PPMImageWriter.h
	${DIR}/src/out/ppm/PPMImageWriter.cpp
	${DIR}/src/out/ppm/PPMImageFileWriter.h
	${DIR}/src/out/ppm/PPMImageFileWriter.cpp
	${DIR}/src/out/ppm/PPMColor.h

	${DIR}/src/sampling/Raygen.h
	${DIR}/src/sampling/Camera.h
	${DIR}/src/sampling/Ray.h
	${DIR}/src/sampling/Lighting.h
	${DIR}/src/sampling/Triangle.h
	${DIR}/src/sampling/IntersectionData.h
	${DIR}/src/sampling/Vertex.h
	${DIR}/src/sampling/Material.h
	${DIR}/src/sampling/Object.h
	${DIR}/src/sampling/Tracer.h

	${DIR}/src/utils/TypeDefs.h

	${DIR}/src/scene/Scene.h

	${DIR}/src/optimisations/ThreadPool.h
	${DIR}/src/optimisations/Bucket.h
	${DIR}/src/optimisations/BucketRenderer.h
	${DIR}/src/optimisations/AABB.h
	${DIR}/src/optimisations/AABBTree.h

	${DIR}/src/math/Vec3.h
)

add_dependencies(${PROJECT_NAME} rapidjson)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_BINARY_DIR} ${DIR}/lib ${DIR}/src ${RAPIDJSON_INCLUDE_DIR}) # for resolving includes