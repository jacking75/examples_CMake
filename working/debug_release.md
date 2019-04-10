# 디버그판 빌드와 릴리즈 빌드판 빌드 하기
출처: https://www.wagavulin.jp/entry/2011/11/27/222642
  
  
## 디버깅 옵션을 붙인다
많은 프로젝트에서 디버그 빌드와 릴리스 빌드를 준비하고 있을 것이다. 

디버그 빌드는 디버그 기호를 붙이고, 또 디버거에 의한 추적이 쉽도록 최적화 옵션을 사용하지 않도록 한다.

한편 릴리스 빌드에서는 반대로 디버그 기호를 붙이지 않고 최적화도 최대한 할 것 이다.



CMake는 변수 CMAKE_BUILD_TYPE를 설정하여 디버그 버전/릴리스 버전의 설정을 실시한다. 

변수의 설정은 CMakeLists.txt 중에서 set 명령으로 설정하는 방법과 명령 줄 옵션으로하는 방법이 있다.



CMakeLists.txt 중에서 지정

cmake_minimum_required (VERSION 2.8)
add_executable (myapp main.cpp)
set (CMAKE_BUILD_TYPE Debug)


명령 줄 옵션에서 지정

> cmake -DCMAKE_BUILD_TYPE = Debug.


또 Debug, Release 외에도 RelWithDebInfo과 MinSizeRel 등도있다. 

자세한 내용은 CMake의 Wiki의 Compilers and Tools 절을 참조하자. 

여기를 보면, 예를 들어 RelWithDebInfo 때는 CMAKE_C_FLAGS_RELWITHDEBINFO 또는 CMAKE_CXX_FLAGS_RELWITHDEBINFO 붙는 것 등을 알 수있다.

### 변수의 내용을 검사
그럼 CMAKE_CXX_FLAGS_RELWITHDEBINFO 에는 무엇이 설정 되어 있을까? 이런 것은 실제로 불러 보면 알 수 있다. 

message 명령을 사용하면 메시지를 표시 할 수 있으며 변수의 내용은 ${변수 명}에서 참조 할 수 있다. 즉, 다음과 같이 쓰고 cmake를 실행하면 표시된다.

cmake_minimum_required (VERSION 2.8)
add_executable (myapp main.cpp)
message (STATUS "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
> cmake.
- -O2 -g
- Configuring done
- Generating done
- Build files have been written to : / home / ws / study-cmake / test
RelWithDebInfo는 -O2 -g가 지정 되는 것 같다. 변수의 표시는 CMakeLists.txt 자체를 확인하고자 할 때 유용하기 때문에 기억해두면 좋다고 생각한다.

명령 및 옵션을 지정
CMake가 자동으로 설정한 컴파일러 또는 옵션 등이 원하지 않으면 수동으로 설정하게 된다. 

이것도 set 명령과 -D 옵션으로 지정한다. 어떤 변수가 있을지는 역시 Wiki의 CMake Useful Variables 이 도움이 될 것이다.