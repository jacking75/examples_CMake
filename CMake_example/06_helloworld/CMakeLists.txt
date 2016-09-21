PROJECT(hello)

set(CMAKE_CXX_COMPILER g++)
add_definitions("-Wall -std=c++14") 

ADD_EXECUTABLE(main main.cpp)

