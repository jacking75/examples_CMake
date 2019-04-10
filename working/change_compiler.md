# CMake의 -C 옵션으로 컴파일러를 전환  
출처: https://qiita.com/yoyomion/items/7974bb093038c21db26d  
  
기본적으로 gcc를 사용하지만 가끔 clang으로 빌드하고 싶은 경우에 사용하는 방법.

## 확인 환경
Ubuntu16.04 (를 VirtualBox에서 돌리는 것)

## 파일 배치

Direcroty 구성

.
├── Qiita
│ ├── CMakeLists.txt
│ └── main.cpp
└── clang-cmakeinit.cmake

Qiita / CMakeLists.txt

cmake_minimum_required(VERSION 3.5)
 
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
 
add_executable(Hello ${SOURCE_FILES})


Qiita / main.cpp

#include <iostream>
 
int main(void){
    std::cout << "Hello World!" << std::endl;
}


clang - cmakeinit.cmake

set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE string "clang compiler" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE string "clang++ compiler" FORCE)


사용법
기본 (gcc)에서 빌드
```
mkdir build
cd build
cmake ../Qitta
make
```


-C 옵션을 사용하여 clang 빌드
```
mkdir build
cd build
cmake -C ../clang-cmakeinit.cmake ../Qiita/
```

참고
cmake 공식 도움말