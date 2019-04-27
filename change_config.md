# 조금씩 설정을 바꾸어서 같은 소스 코드를 빌드하고 싶을 때
출처: https://qiita.com/yumetodo/items/bd8f556ab56298f19ba8  
  
벤치마크 등에서 매크로 정의를 교체하여 같은 소스 코드에서 복수의 binary를 토해내는 경우가 있다.  
https://github.com/yumetodo/qsort  
  
directory 구성은  
<pre>
/project root
├── benchmark.sh
├── CMakeLists.txt
├── main_prog.cpp
├── mm88.h
├── mm88c.c
├── qs10a5.c
├── qs10a5.h
├── qs9e17.c
├── qs9e17.h
├── random_device.hpp
├── qs10a5
│   └── CMakeLists.txt
├── qs10a5m
│   └── CMakeLists.txt
├── qs9e17
│   └── CMakeLists.txt
└── qsort
    ├── CMakeLists.txt
    └── main.cpp
</pre>
  
아래처럼 기술하면 루트 디렉토리에서 cmake 하면 전부 모아서 빌드한다.  
/CMakeLists.txt    
```
cmake_minimum_required(VERSION 3.3)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
 
#
# Update compiler waring flags
#
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()
 
add_subdirectory(qsort)
add_subdirectory(qs9e17)
add_subdirectory(qs10a5)
add_subdirectory(qs10a5m)
 
install(
    FILES "./benchmark.sh"
    PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    DESTINATION "."
)
```
  
/qs10a5/CMakeLists.txt   
```
set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../mm88c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../qs10a5.c"
)
add_definitions(-DUSE_QS10A5)
add_executable(qs10a5 ${SRCS})
 
install(TARGETS qs10a5 RUNTIME DESTINATION ".")
```
  
  
/qs10a5m/CMakeLists.txt  
```
set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../mm88c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../qs10a5.c"
)
add_definitions(-DUSE_QS10A5)
add_executable(qs10a5 ${SRCS})
 
install(TARGETS qs10a5 RUNTIME DESTINATION ".")
```
  
  
/qs10a5m/CMakeLists.txt  
```
set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../mm88c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../qs10a5.c"
)
add_definitions(-DUSE_QS10A5 -DMEMCPY)
add_executable(qs10a5m ${SRCS})
 
install(TARGETS qs10a5m RUNTIME DESTINATION ".")
```
  
  
/qs9e17/CMakeLists.txt  
```
set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../mm88c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../qs9e17.c"
)
add_definitions(-DUSE_QS9E17)
add_executable(qs9e17 ${SRCS})
 
install(TARGETS qs9e17 RUNTIME DESTINATION ".")
```
  
  
/qsort/CMakeLists.txt  
```
set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
)
add_definitions(-DUSE_STDLIB_QSORT)
add_executable(qsort ${SRCS})
 
install(TARGETS qsort RUNTIME DESTINATION ".")
```
  
  
```
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX="../bin" -DCMAKE_BUILD_TYPE=Release ..
$ make install
$ cd ../bin
```
  