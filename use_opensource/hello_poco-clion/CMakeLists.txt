cmake_minimum_required(VERSION 3.9)
project(hello_poco_clion)

set(CMAKE_CXX_STANDARD 17)


set(POCO_ROOT $ENV{HOME}/dev/cpp/thirdparty/poco)
set(Poco_Foundation_INCLUDE_DIR ${POCO_ROOT}/Foundation/include)
set(Poco_LIBRARY_DIR ${POCO_ROOT}/lib/Linux/x86_64)

FIND_LIBRARY(Poco_LIBRARIES NAMES PocoFoundation PocoNet PATH_SUFFIXES ${Poco_LIBRARY_DIR})

include_directories(${Poco_Foundation_INCLUDE_DIR})
link_directories(${Poco_LIBRARY_DIR})


add_executable(hello_poco_clion main.cpp)
target_link_libraries( hello_poco_clion PocoFoundation64 )