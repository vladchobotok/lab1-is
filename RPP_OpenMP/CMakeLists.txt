cmake_minimum_required(VERSION 3.27)
project(RPP_OpenMP)

# Find OpenMP package
find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 23)

add_executable(RPP_OpenMP main.cpp)

# Enable OpenMP support
if (OpenMP_C_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
    target_link_libraries(RPP_OpenMP PRIVATE ${OpenMP_C_LIBRARIES})
endif ()
