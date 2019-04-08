cmake_minimum_required(VERSION 3.9)
project(hello_boost_clion)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT $ENV{HOME}/dev/cpp/thirdparty/boost)
set(Boost_INCLUDE_DIR ${BOOST_ROOT})
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/stage/lib)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
link_libraries(boost_regex)

add_executable(hello_boost_clion main.cpp)
#target_link_libraries( hello_boost_clion ${Boost_LIBRARIES} )