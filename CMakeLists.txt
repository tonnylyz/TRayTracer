cmake_minimum_required(VERSION 3.10)
project(raytracer)
set(CMAKE_CXX_STANDARD 11)
include_directories(../include)
link_directories(../lib)

set(RAYTRACER_SOURCE
        main.cpp
        camera.h
        directional_light.h
        geometry.h
        light.h
        point_light.h
        sphere.h
        string_util.h
        transform.h
        triangle.h
        ray.h intersection.h ray.cpp light.cpp transform.cpp intersection.cpp geometry.cpp math_util.h camera.cpp)

add_executable(raytracer ${RAYTRACER_SOURCE})
target_link_libraries(raytracer FreeImage)