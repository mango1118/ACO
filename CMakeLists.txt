cmake_minimum_required(VERSION 3.24)
project(ACO)

set(CMAKE_CXX_STANDARD 17)

include_directories(./EasyX/include)
link_directories(./EasyX/lib64)
link_libraries(libeasyx.a)

add_executable(ACO ACO.cpp ACO.h Ant.cpp Ant.h Graph.cpp Graph.h AntColony.cpp AntColony.h Axis.cpp)
