cmake_minimum_required(VERSION 3.9)
project(echoServer_boostAsio)

set(CMAKE_CXX_STANDARD 17)

set(asioLib_LIBRARY_DIR $ENV{HOME}/com2usRespo/techdocs/cpp/samples/boostAsioLib)
set(asioLib_LIBRARY_OUTPUT_DIR $ENV{HOME}/com2usRespo/techdocs/cpp/samples/output)

set(BOOST_ROOT $ENV{HOME}/dev/cpp/thirdparty/boost)
set(Boost_INCLUDE_DIR ${BOOST_ROOT})
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/stage/lib)

include_directories(
        ${Boost_INCLUDE_DIR}
        ${asioLib_LIBRARY_DIR}
)

link_directories(
        {Boost_LIBRARY_DIR}
        ${asioLib_LIBRARY_OUTPUT_DIR}
)

link_libraries(pthread boost_system boost_thread boostAsioLib)

add_executable(echoServer_boostAsio main.cpp)
