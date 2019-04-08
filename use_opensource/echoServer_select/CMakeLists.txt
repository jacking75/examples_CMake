cmake_minimum_required(VERSION 3.9)
project(echoServer_select)

set(CMAKE_CXX_STANDARD 17)

set(netLib_LIBRARY_DIR $ENV{HOME}/com2usRespo/techdocs/cpp/samples/netLibSelect)
set(netLib_LIBRARY_OUTPUT_DIR $ENV{HOME}/com2usRespo/techdocs/cpp/samples/output)

include_directories(
        ${netLib_LIBRARY_DIR}
)

link_directories(${netLib_LIBRARY_OUTPUT_DIR})

link_libraries(netLibSelect)

message( STATUS "LIB_DIR:" ${netLib_LIBRARY_DIR} )
message( STATUS "LIB_OUTPUT_DIR:" ${netLib_LIBRARY_OUTPUT_DIR} )

add_executable(echoServer_select main.cpp)
