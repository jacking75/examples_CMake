cmake_minimum_required(VERSION 3.9)
project(netLibSelect)

set(CMAKE_CXX_STANDARD 17)

add_library(netLibSelect ILog.h Define.h ITcpNetwork.h ServerNetErrorCode.h
            ServerNetErrorCode.h TcpNetwork.cpp TcpNetwork.h
        BasicLog.h)

set( CMAKE_LIBRARY_OUTPUT_DIRECTORY  ../../output )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ../../output )

set_target_properties( netLibSelect
        PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ../../output
        ARCHIVE_OUTPUT_DIRECTORY ../../output
       )