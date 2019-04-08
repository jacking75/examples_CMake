cmake_minimum_required(VERSION 3.9)
project(boostAsioLib)

set(CMAKE_CXX_STANDARD 17)

set(BOOST_ROOT $ENV{HOME}/dev/cpp/thirdparty/boost)
set(Boost_INCLUDE_DIR ${BOOST_ROOT})
set(Boost_LIBRARY_DIR ${BOOST_ROOT}/stage/lib)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_library(boostAsioLib network.cpp network.h)

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY  ../../output )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ../../output )

set_target_properties( boostAsioLib
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ../../output
        ARCHIVE_OUTPUT_DIRECTORY ../../output
        )