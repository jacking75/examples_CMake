PROJECT(hello)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-m64")
add_definitions("-Wall -std=c++14") 

INCLUDE_DIRECTORIES(/$ENV{HOME}/Dev/C++/ThirdParty/boost_1_60_0)
LINK_DIRECTORIES(/$ENV{HOME}/Dev/C++/ThirdParty/boost_1_60_0/stage/gcc/lib)

ADD_EXECUTABLE(hello-boost hello-boost.cpp)

TARGET_LINK_LIBRARIES(hello-boost pthread boost_thread boost_system boost_chrono)


