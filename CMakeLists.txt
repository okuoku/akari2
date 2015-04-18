project(akari2)
cmake_minimum_required(VERSION 2.8)

find_package(OpenMP REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")

add_executable(akari2
    # CXX
    bmpexporter.cpp
    main.cpp

    # H
    bbox.h
    bmpexporter.h
    brdf.h
    constant.h
    hmath.h
    hon.h
    ibl.h
    image.h
    intersection.h
    kdtree.h
    memfile.h
    objMesh.h
    qbvh.h
    random.h
    ray.h
    sampling.h
    textutil.h
    timer.h
    triangle.h
    triangleMesh.h
    vec3.h
    )
