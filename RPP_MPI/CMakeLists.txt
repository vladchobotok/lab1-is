cmake_minimum_required(VERSION 3.27)
project(RPP_MPI)

set(CMAKE_CXX_STANDARD 23)

# Add MPI Package to Project
find_package(MPI REQUIRED)

add_executable(RPP_MPI main.cpp)

# Link MPI libraries
target_link_libraries(RPP_MPI PRIVATE MPI::MPI_CXX)
