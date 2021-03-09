cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(SocketUtility)


set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/x64/Debug/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/x64/Release/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/x64/Debug/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/x64/Release/)

include_directories(src)

add_executable(tcpserver samples/tcpserver.cpp src/socketbase.cpp)
add_executable(tcpclient samples/tcpclient.cpp src/socketbase.cpp)
add_executable(udpserver samples/udpserver.cpp src/socketbase.cpp)
add_executable(udpclient samples/udpclient.cpp src/socketbase.cpp)

