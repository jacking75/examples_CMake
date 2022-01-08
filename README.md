# CMake 예제

## 예제 소개
- 예제 코드는 CMake_example 디렉토리에 있다.  
  
  
### 01 helloworld - 간단한 1개 파일의 C++ 코드
main.cpp   
```
#include <iostream>

int main()
{
  auto name = "jacking";
  std::cout << "hello world: " << name << std::endl;
  return 0;
}
```
  
CMakeLists.txt  
```
cmake_minimum_required(VERSION 2.8)
add_executable(Main main.cpp)
```
  
  
  
### 02 helloworld - 컴파일러 옵션을 설정. -Wall, C++14
main.cpp  
```
#include <iostream>

int main()
{
  auto name = "jacking";
  std::cout << "hello world: " << name << std::endl;
  return 0;
}
```
  
CMakeLists.txt  
```
cmake_minimum_required(VERSION 2.8)
add_definitions("-Wall -std=c++14")
add_executable(Main main.cpp)
```
  
  

### 03 helloworld - main 코드 파일 이외의 코드 파일이 있는 경우
main.cpp  
```
#include "test.h"

int main()
{
    TEST test;
    test.Print();
    return 0;
}
```  

test.h  
```
class TEST
{
public:
    void Print();
};
```
  
test.cpp  
```
#include "test.h"

#include <iostream>

void TEST::Print()
{
    std::cout << "Test::Print" << std::endl;
}
```
  
CMakeLists.txt  
```
cmake_minimum_required(VERSION 2.8)
add_executable(Main
  main.cpp
  test.cpp
)
```
  
  

### 04 helloworld - mai.cpp 이외의 파일이 다른 디렉토리에 있는 경우  
main.cpp  
```
#include "test01/test01.h"
#include "test02/test02.h"

int main()
{
  TEST01 test01;
  test01.Print();

  TEST02 test02;
  test02.Print();
  return 0;
}
```
  
test01  
```
class TEST01
{
public:
  void Print();
};
```
  
```
#include "test01.h"
#include <iostream>

void TEST01::Print()
{
    std::cout << "Test01::Print" << std::endl;
}
```
  
test02  
```
class TEST02
{
public:
    void Print();
};
```
  
```
#include "test02.h"
#include <iostream>

void TEST02::Print()
{
    std::cout << "Test02::Print" << std::endl;
}
```
  
CMakeLists.txt
```
cmake_minimum_required(VERSION 2.8)
add_executable(Main
  main.cpp
  test01/test01.cpp
  test02/test02.cpp
)
```
  
  

### 05 helloworld - static 파일을 만든 후 참조  
main.cpp   
```
#include "test01/test01.h"
#include "test02/test02.h"

int main()
{
    TEST01 test01;
    test01.Print();

    TEST02 test02;
    test02.Print();
    return 0;
}
```
  
CMakeLists.txt  
```
cmake_minimum_required(VERSION 2.8)
add_subdirectory(test01)                
add_subdirectory(test02)                
add_executable(Main main.cpp)
target_link_libraries(Main Test01 Test02)
```
  
test01  
```
class TEST01
{
public:
    void Print();
};
```
  
```
#include "test01.h"

#include <iostream>

void TEST01::Print()
{
    std::cout << "Test01::Print" << std::endl;
}
```
  
```
cmake_minimum_required(VERSION 2.8)
add_library(Test01 STATIC
  test01.cpp
)

```
  
test02  
```
class TEST02
{
public:
    void Print();
};
```
  
```
#include "test02.h"

#include <iostream>

void TEST02::Print()
{
    std::cout << "Test02::Print" << std::endl;
}
```
  
```
cmake_minimum_required(VERSION 2.8)
add_library(Test02 STATIC
  test02.cpp
)
```
  
  

### 06 helloworld - 컴파일러 지정  
main.cpp  
```
#include <iostream>

int main()
{
    auto name = "jacking";
  std::cout << "hello world: " << name << std::endl;
  return 0;
}
```
  
CMakeLists.txt  
```
PROJECT(hello)

set(CMAKE_CXX_COMPILER g++)
add_definitions("-Wall -std=c++14")

ADD_EXECUTABLE(main main.cpp)
```
  
  

### 07 helloworld - 외부 라이브러리(여기서는 Boost 라이브러리) 사용하기  
main.cpp  
```
#include <boost/thread.hpp>
#include <iostream>

int main()
{
    std::cout << "Boost.Thread !!!" << std::endl;
    boost::thread Thread1( [] ()
    {
        for( int i = 0; i < 5; ++i )
        {
            std::cout << "Thread Num : " << i << std::endl;
        }
    } );

    Thread1.join();
    return 0;
}
```
  
CMakeLists.txt  
```
PROJECT(hello)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-m64")
add_definitions("-Wall -std=c++14")

INCLUDE_DIRECTORIES(/$ENV{HOME}/Dev/C++/ThirdParty/boost_1_60_0)
LINK_DIRECTORIES(/$ENV{HOME}/Dev/C++/ThirdParty/boost_1_60_0/stage/gcc/lib)

ADD_EXECUTABLE(hello-boost hello-boost.cpp)

TARGET_LINK_LIBRARIES(hello-boost pthread boost_thread boost_system boost_chrono)
```
  
  

## 사용법
### 변수 다루기
cmake 에서는 변수를  
<pre>
set(HOMHOM "hom")
</pre>  
처럼 지정할 수 있다.
  
이렇게 지정한 변수는 그보다 아래의 아래에는 전해지지만 위로는 전해지지 않는다.  
위로 전하고 싶은 경우는 CACHE 지정자를 사용한다.  
<pre>
set(HOMHOM "hom" CACHE STRING "homhom is hommm")
</pre>
  
이 기능을 이용하여 가장 부모 디렉토리에서  
<pre>
set(CMAKE_BUILD_TYPE Release)
</pre>
  
로 해두고, 테스트 용 디렉토리에서는  
<pre>
set(CMAKE_BUILD_TYPE Debug)
</pre>
 
로 하는 것이 가능하다.  
  
  

### 컴파일러 링커에 대한 기본적인 설정
출처: https://www.wagavulin.jp/entry/2011/11/27/222642
  
#### 소스 파일을 증가
보통 프로젝트에서 소스 파일이 하나라는 것은 없을 것이다. 소스 파일을 늘리려면 단순히 add_executable 명령에 추가하면 된다.
```
add_executable (myapp main.cpp sub1.cpp sub2.cpp)
```  
이정도라면 어떻게 의미없는 이야기이지만, 동일한 대상에 대해 add_executable는 1회 밖에 부를 수없는 것에주의. 즉,
```
add_executable (myapp main.cpp)
add_executable (myapp sub1.cpp sub2.cpp)
```  
같은 것은 없다.   
CMake도 조건 분기를 수행하는 if 명령이 있지만(자세한 것은 후술) 조건에 따라 소스 파일을 추가하려면 이래서는 곤란하다. 다음과 같이 소스 파일은 변수에 넣어 두었다가 마지막에 정리해 놓은 것이 좋겠다.  
```
set (myapp_sources main.cpp sub1.cpp)
if (USE_FEATURE_X)
  set (myapp_sources ${myapp_sources} sub2.cpp)
endif ()
add_executable (myapp ${myapp_sources})
```
  

### 인클루드 패스를 지정
include 경로를 지정할 때 include_directories 명령을 사용한다.
```
cmake_minimum_required (VERSION 2.8)
include_directories (/path/to/include)
add_executable (myapp main.cpp)
```

또한 include_directories는 여러번 지정할 수 있으며, 기본적으로 나중에 지정한 것은 뒤에 붙는다. 그러나 옵션 BEFORE를 지정했을 때는 앞에 붙는다. 예를 들어,
```
include_directories (/path1/include)
include_directories (/path2/include)
```
로 했을 때는 "-I/path1/include -I/path2/include"가 되고,
```
include_directories (/path1/include)
include_directories (BEFORE/path2/include)
```
로 했을 때는 "-I/path2/include -I/path1/include"가 된다.
  
  
### 매크로 지정
매크로를 지정할 때 add_definitions을 사용한다.
```
add_definitions (-DFOO -DBAR = xyz)
```  
include_directories 때 단순히 디렉토리 이름을 지정하면 -I는 붙지 않았지만, add_definitions 때는 -D를 붙일 필요가 있다는 점에 유의.   
매크로를 지정할뿐만 아니라 정의를 제거(-U)을 사용하는 것도 가능하기 때문이다. 
```
add_definitions (-DFOO -UHOGE)
```
  
덧붙여서 -D이나 -U 부분은 환경에 맞게 적절하게 바꾸어 준다. 예를 들어 Visual C++ 용으로 생성하려면 제대로 /D 및 / U가 설정된다.
또한 add_definitions도 여러 번 부를 수 있다. 또한 같은 매크로를 여러 번 정의하면 그대로 여러 번 정의 된 것이 생성되고 CMake 자체는 경고 등은 내지 않는다. 
그러나 대부분의 컴파일러는 이러한 매크로 정의에 대해 경고를 낸다.
   
   
### 라이브러리 추가
링크 라이브러리를 추가 할 때 target_link_libraries 이다. 결과는 -l 옵션에 반영된다.
```
cmake_minimum_required (VERSION 2.8)
add_executable (myapp main.cpp)
target_link_libraries (myapp foo)
```  
라이브러리 이름뿐만 아니라 대상 이름도 지정해야 한다. 따라서 target_link_libraries는 add_executable 이후에 있어야 하며 먼저 지정하면 에러가 된다.
  

### 라이브러리 경로 지정
링크하는 라이브러리가 있는 경로를 지정하려면 link_directories 명령을 사용한다.  
```
cmake_minimum_required (VERSION 2.8)
link_directories (/path/to/lib)
add_executable (myapp main.cpp)
```
  
물론, 결과는 -L 옵션에 반영된다. 주의 사항으로는 link_directories 이때까지 이미 생성된 대상만 효과가 있다는 것이다.  
앞의 예에서 말하면, link_directories 행과 add_executable의 행을 바꾸면 무효가 된다. 그리고 target_link_libraries의 경우와 달리, 먼저 배치하면 오류 경고가 나오지  않는다.  
덧붙여서 include_directories과 add_definitions은 어디에 두어도 유효된다.  
    

### 정리
명령 이름이 기억하기 어렵기 때문에 정리해 둔다.  
  
소스 파일	add_executable	add_executable (myapp src1.cpp src2.cpp)	소스 파일은 변수로 일단 변수에 넣어 두자.   
인클루드 패스	include_directories	include_directories (/path/to/include)	CMakeLists.txt 중 어디에 두어도 사용.   
라이브러리 경로	link_directories	link_directories (/path/to/lib)	이후에 생성된 대상에 대해서만 유효하다.  
라이브러리	target_link_libraries	target_link_libraries (myapp foo)	대상 작성 후 없으면 지정할 수 없다.  
    
    
부르는 순서는 우선 다음과 같이 해두면 좋을 것이다.  
```
cmake_minimum_required (VERSION 2.8)
add_definitions (-DFOO)
include_directories (/ path1 / to / include / path2 / to / include)
link_directories (/ path1 / to / lib / path2 / to / lib)
add_executable (myapp main.cpp sub1.cpp)
target_link_libraries (foo bar)
```  
또한 include_directories, link_directories, target_link_libraries 는 여러 번 사용하여 추가 할 수 있다.  
      
  	
  
## 출력 디렉토리 지정  
[how do I make cmake output into a 'bin' dir? - StackOverflow](http://stackoverflow.com/questions/6594796/how-do-i-make-cmake-output-into-a-bin-dir)    
실행 파일  
```
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../libs)
```
  
정적 라이브러리  
```
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../../libs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../../../libs)
```
  
  
## CMake의 -C 옵션으로 컴파일러를 전환  
출처: https://qiita.com/yoyomion/items/7974bb093038c21db26d  
기본적으로 gcc를 사용하지만 가끔 clang으로 빌드하고 싶은 경우에 사용하는 방법.  
  
### 확인 환경
Ubuntu16.04 (를 VirtualBox에서 돌리는 것)  
  
### 파일 배치
Direcroty 구성  
.  
├── Qiita  
│ ├── CMakeLists.txt  
│ └── main.cpp  
└── clang-cmakeinit.cmake  
  
Qiita / CMakeLists.txt  
```
cmake_minimum_required(VERSION 3.5)
 
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
 
add_executable(Hello ${SOURCE_FILES})
```
  
Qiita / main.cpp  
```
#include <iostream>
 
int main(void){
    std::cout << "Hello World!" << std::endl;
}
```
  
clang - cmakeinit.cmake  
```
set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE string "clang compiler" FORCE)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE string "clang++ compiler" FORCE)
```
  
### 사용법
#### 기본 (gcc)에서 빌드  
```
mkdir build
cd build
cmake ../Qitta
make
```
  
  
#### -C 옵션을 사용하여 clang 빌드
  
```
mkdir build
cd build
cmake -C ../clang-cmakeinit.cmake ../Qiita/
```
  
  

## CMAKE_CXX_FLAGS이에 대응하는 CMake 명령
set에서 다음과 같이 정리해서 옵션을 줄 수 있다.  
```
set( CMAKE_CXX_FLAGS "-O2 -std=c++11 -Wall ..." )
```
  
(CMake의 변수는 [CMake useful variables](https://cmake.org/Wiki/CMake_Useful_Variables)를 참조하는 것이 가장 빠르다)  
   
자, 그럼 이 경우 set 명령을 어디에 두어야할까? 이것을 결정하려면 CMake에서 변수의 범위를 알 필요가 있다.    
부모 디렉토리에 정의되어 있으면 하위 디렉토리에서도 사용할 수 있다.   
최적화 등의 옵션은 빌드 전반에 걸쳐 동일한 것을 사용하기 때문에 프로젝트 이름을 정의한 후 서브 디렉토리를 등록하기 전에 두면 하위 디렉토리에서의 처리에도 같은 옵션이 적용된다.  
  
실제로 다양한 위치를 바꾸어 빌드하고, make VERBOSE=1 로 빌드 세부 사항을 토해 내게 해 보았지만, set을 둔 이후로 밖에 사용되지 않았기 때문에 해당 옵션이 필요한 처리 전에 오도록 배치해야 한다.  
  
/CMakeLists.txt  
```
cmake_minimum_required ( VERSION 2.8 ) 
project ( test_cmake CXX ) 
set ( CMAKE_CXX_FLAGS "-O2 -std = c ++ 11 -Wall" ) 
add_subdirectory ( src ) 
add_subdirectory ( test )
```
    
### CXX_STANDARD
CMake 3.1에서 std 옵션을 CMAKE_CXX_STANDARD을 사용하여 지정할 수 있게 되었다.   
 
/CMakeLists.txt  
```
cmake_minimum_required ( VERSION 3.1 ) 
project ( test_cmake CXX ) 
set ( CMAKE_CXX_STANDARD 11 ) 
set ( CMAKE_CXX_FLAGS "-O2 -Wall" ) 
add_subdirectory ( src ) 
add_subdirectory ( test )
```
    
### 빌드의 종류에 따라 옵션을 변경
디버그 모드로 빌드하고 싶을 때 등 일일이 CMakeLists.txt의 내용을 변경하는 것은 번거롭다.   
그래서 CMake는 미리 빌드의 종류에 따라 별도의 옵션을 정의 해두고 빌드 할 때 지정할 수 있도록 되어 있다.  
   
/CMakeLists.txt  
```
cmake_minimum_required ( VERSION 3.1 ) 
project ( test_cmake CXX ) 
set ( CMAKE_CXX_STANDARD 11 ) 
set ( CMAKE_CXX_FLAGS "-O1 -Wall" ) 
set ( CMAKE_CXX_FLAGS_DEBUG "-O0 -g" ) 
set ( CMAKE_CXX_FLAGS_RELEASE "-O2" ) 
set ( CMAKE_CXX_FLAGS_MINSIZEREL "-Os" ) 
set ( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Og -g" ) 
add_subdirectory ( src ) 
add_subdirectory( test )
```  
로 하면 CMake를 실행할 때 -DCMAKE_BUILD_TYPE을 지정 해주면 지정된 모드의 플래그를 사용하여 빌드 해준다.    
  
<pre>
> cmake ..                                     # 기본 
> cmake -DCMAKE_BUILD_TYPE = Debug ..            # 디버그 
> cmake -DCMAKE_BUILD_TYPE = Release ..          # 자료 
> cmake -DCMAKE_BUILD_TYPE = MinSizeRel ..       # 최소 크기 자료 
> cmake -DCMAKE_BUILD_TYPE = RelWithDebInfo ..   # 디버깅 정보를 더 해서  출시
</pre>
   
각 파일을 컴파일 할 때 CMAKE_CXX_FLAGS 뒤에 각 모드의 플래그가 추가된다.  
여러 충돌 하는 옵션이 있을 경우 나중의 옵션이 채용된다.  
또한 코드의 프로파일을 실시하는 등 일시적으로 옵션을 추가하려면 CMake를 실행할 때 프래그를 전달하여 추가 할 수 있다.  
  
<pre>  
> cmake -DCMAKE_CXX_FLAGS = "-pg" ..
</pre>
  
그러나 이렇게 하려면 플래그 작성을 다음과 같이 해야한다.    
  
/CMakeLists.txt  
```
set ( CMAKE_CXX_FLAGS " $ { CMAKE_CXX_FLAGS } -O1 -Wall" )
```
  
이렇게 하면 명령 줄에서 주어진 옵션이 추가 될 수 있다.  
  
g++의 기본 설정
- Default :
- Debug : -g
- Release : -O3 -DNDEBUG
- MinSizeRel : -Os -DNDEBUG
- RelWithDebInfo : -O2 -g -DNDEBUG  
  
  
  
## 디버그판 빌드와 릴리즈 빌드판 빌드 하기
출처: https://www.wagavulin.jp/entry/2011/11/27/222642  
    
### 디버깅 옵션을 붙인다
많은 프로젝트에서 디버그 빌드와 릴리스 빌드를 준비하고 있을 것이다.   
디버그 빌드는 디버그 기호를 붙이고, 또 디버거에 의한 추적이 쉽도록 최적화 옵션을 사용하지 않도록 한다.  
한편 릴리스 빌드에서는 반대로 디버그 기호를 붙이지 않고 최적화도 최대한 할 것 이다.  
  
CMake는 변수 CMAKE_BUILD_TYPE를 설정하여 디버그 버전/릴리스 버전의 설정을 실시한다.   
변수의 설정은 CMakeLists.txt 중에서 set 명령으로 설정하는 방법과 명령 줄 옵션으로하는 방법이 있다.  
  
CMakeLists.txt 중에서 지정  
```
cmake_minimum_required (VERSION 2.8)
add_executable (myapp main.cpp)
set (CMAKE_BUILD_TYPE Debug)
```
  
명령 줄 옵션에서 지정  
<pre>
> cmake -DCMAKE_BUILD_TYPE = Debug.
</pre>
  
또 Debug, Release 외에도 RelWithDebInfo과 MinSizeRel 등도 있다.   
자세한 내용은 CMake의 Wiki의 Compilers and Tools 절을 참조하자.   
여기를 보면, 예를 들어 RelWithDebInfo 때는 CMAKE_C_FLAGS_RELWITHDEBINFO 또는 CMAKE_CXX_FLAGS_RELWITHDEBINFO 붙는 것 등을 알 수있다.  
  

### 변수의 내용을 검사
그럼 CMAKE_CXX_FLAGS_RELWITHDEBINFO 에는 무엇이 설정 되어 있을까? 이런 것은 실제로 불러 보면 알 수 있다.   
message 명령을 사용하면 메시지를 표시 할 수 있으며 변수의 내용은 ${변수 명}에서 참조 할 수 있다. 즉, 다음과 같이 쓰고 cmake를 실행하면 표시된다.  
```
cmake_minimum_required (VERSION 2.8)
add_executable (myapp main.cpp)
message (STATUS "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
```
  
<pre>  
> cmake.
- -O2 -g
- Configuring done
- Generating done
- Build files have been written to : / home / ws / study-cmake / test
</pre>
  
RelWithDebInfo는 -O2 -g가 지정 되는 것 같다. 변수의 표시는 CMakeLists.txt 자체를 확인하고자 할 때 유용하기 때문에 기억해두면 좋다고 생각한다.  
  
명령 및 옵션을 지정  
CMake가 자동으로 설정한 컴파일러 또는 옵션 등이 원하지 않으면 수동으로 설정하게 된다.   
이것도 set 명령과 -D 옵션으로 지정한다. 어떤 변수가 있을지는 역시 Wiki의 CMake Useful Variables 이 도움이 될 것이다.  
  
  
 
## 소스 밖에서 빌드
[출처](https://www.wagavulin.jp/entry/2011/11/27/222658 )  
  
CMake를 사용하면 CMakeCache.txt과 CMakeFiles 등 다양한 파일이나 디렉토리가 생성된다.  
결과적으로 프로젝트의 디렉토리가 잘 보이지 않는 경향이 있다.   
이럴 때는 빌드 작업 디렉토리를 다른 디렉토리로 하면 좋다.   
이것을 소스 외부 빌드 (out-of-source build)라고 부른다.  
예를 들어, CMakeLists.txt와 main.cpp가 myapp 디렉토리에있을 때 myapp에 build 디렉토리를 만들고 이 속에서 빌드하도록 하자.  
<pre>
myapp /
  + - CMakeLists.txt
  + - main.cpp
  + - build / #이 안에서 구축
</pre>
  
방식은 특히 어렵지 않다. build 디렉토리를 만들고 이 속에서 cmake를 부를 뿐이다. 이 때 CMakeLists.txt이 있는 디렉토리를 지정하면 된다.  
이렇게하면 my-project 디렉토리에 파일 디렉토리의 추가 · 변경은 되지 않는다. 필요 없게 되면 build 디렉토리를 지운면 된다.  
<pre>
> ls
CMakeLists.txt main.cpp
> mkdir build # build 디렉토리를 만든다
> cd build / # 이 안에서 빌드
> cmake .. && make && ./myapp
(snip)
hello
</pre>
  
소스 외 빌드 기능을 사용하면 빌드 디렉토리를 여러 만들 수있다.   
이렇게 하면 옵션에 대해 다른 디렉토리를 사용할 수 있게 된다. 예를 들어, 디버그 버전과 릴리스 버전을 각각 Debug, Release 디렉토리에 하면 좋을 것이다.  
  

### 소스 외 빌드와 상대 경로
소스 외 빌드를 할 경우 주의해야 하는 것이 경로의 설정이다.   
빌드가 이루어지는, 즉 컴파일러 및 링커가 실행 되는 디렉토리가 바뀌기 때문에 CMakeLists.txt이 있는 디렉토리로부터의 상대 패스가 있으면 빌드 하지 못할 수있다.  
예를 들어, 다음과 같은 구성으로 설정 파일을 생성하도록 하였다.  
<pre>
myapp /
  + - CMakeLists.txt
  + - HogeConfig.h.in #이것을 바탕으로 HogeConfig.h를 생성한다
  + - Hoge.h # #nclude "HogeConfig.h"를 포함
  + - Hoge.cpp #
  + - build / # 빌드 디렉토리
</pre>  
  
```  
cmake_minimum_required(VERSION 2.8)
configure_file(
  "HogeConfig.h.in" "HogeConfig.h"
)
  
add_library(libHoge Hoge.cpp)
```
  
이제 build 디렉토리에서 빌드하면 다음과  같이 실패한다.  
<pre>
> cmake ..
(snip)
> ls # HogeConfig.h를 여기에서 할 수  있다
CMakeCache.txt CMakeFiles / HogeConfig.h Makefile cmake_install.cmake
> make VERBOSE = 1
(snip)
/ usr / bin / c ++ -o CMakeFiles / libHoge.dir / Hoge.cpp.o -c /home/ws/wagavulin/06/myapp02/Hoge.cpp
In file included from /home/ws/wagavulin/06/myapp02/Hoge.cpp:1:0 :
/home/ws/wagavulin/06/myapp02/Hoge.h:4:24 : 치명적인 오류 : HogeConfig.h : 이런 파일이나 디렉토리가 없습니다
</pre>
  
따라서 파일을 생성하려면 다음과 같이 쓸 필요가 있다.  
```
configure_file(
  "${PROJECT_SOURCE_DIR}/HogeConfig.h.in"
  "${PROJECT_SOURCE_DIR}/HogeConfig.h"
)
```
  
소스 외 빌드를 하려고 하면 오류가 발생하는 경우는 각종 경로 설정을 검토 해 보자.  
   
  
  
## Tips 
  
### C++14로 컴파일 하기
  
```  
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11
```
  
CMAKE_CXX_STANDARD 에 11, 14, 17 로 지정한다.  
  
CMAKE_CXX_EXTENSIONS을 OFF로 하지 않으면 GNU 확장이 사용된다.   
  
  
### 경고 레벨
Visual C++ 이라면 /W4, GCC/Clang 이라면 -Wall -Wextra 를 붙인다.  
```
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
```
  
  
### 자주 사용하는 옵션
옵션으로 자주 사용하는 것이라면 최적화 옵션 O 나 c ++ 11/14/17 기능을 활성화 하는 -std=c++11/14/1z 와 -Wall 등의 경고 옵션.  
명령 줄에서 빌드하는 경우, 예를 들면 이런 식으로 옵션을 추가한다.  
```
g++ -O2 -std = c ++ 11 -Wall ... (기타 다양한 부가)
```
  
   
### CMake 업그레이드 하기
패키지로 설치 되는 것보다 더 높은 버전을 원한다면 직접 설치해야 한다.  
  
현재(2018.11) 최신 버전은 3.13.0 버전이고, 이 비전의 소스를 다운로드 한다.   
다운로드 주소는 https://cmake.org/download/ 에서 알 수 있다.  
  
다운로드 한다.
```
$ wget https://cmake.org/files/v3.13/cmake-3.13.0.tar.gz
```
  
압축을 푼다  
```
$ tar -zxvf cmake-3.13.0.tar.gz
```  
  
빌드 후 설치한다  
<pre>
$ cd cmake-3.13.0
$ ./bootstrap
$ make
$ sudo make install
</pre>
  
설치는 /usr/local/bin 에 설치된다  
  
  
### Natvis 파일을 VS의 프로젝트에 추가 할 필요가 있을 때
Visual Studio의 디버거에서 표시를 사용자 정의하는 것으로 Natvis가 있다.   
[Create custom views of native objects in the debugger | Microsoft Docs](https://docs.microsoft.com/ko-kr/visualstudio/debugger/create-custom-views-of-native-objects?view=vs-2019 )  
  
.vcxproj 로 이것들을 추가해서 인식시키기 위해서는 
```
<ItemGroup>
    <Natvis Include="inferior_vector.natvis" />
  </ItemGroup>
```
  
위와 같은 것을 쓰면 인식된다. 그럼 CMake에서 이것을 내게 하려면  
```
add_executable(<project name> <source files>... <natvis files>...)
```
  
로 단순하게 add_executable 로 지정해두면 된다. 순서는 상관 없다.  
VS 이외에는 바로 무시되므로 if(MSVC) 라고 쓸 필요도 없다.  
  
CMake 3.7.0 에서 부터이다.
```
cmake_minimum_required(VERSION 3.7.0)
```
  


## Makefile 에서 += 라는 것을 CMake에서 어떻게 기술?
예를들면 Makefile 에서  
```
SRCS += hoge.c
```  
라는 것은   
``` 
set(SRCS ${SRCS} hoge.c)  
```
  
컴파일러 플래그에 세미콜롬(';')을 넣어서 빌드가 중단 되었다.  
CMake는 내부에서는 리스트를 세미콜롬 구별 문자열로 정리하고 있으므로 이것을 컴파일러에 넘기면 세미컬럼이 남겨져 버린다.    
이런 경우 ""로 구별하는 것으로 세미콜롬 구별을 스페이스 구별로 변환해 준다.    
```
CFLAGS += -g
```   
라는 Makefile 에서 쓴 것을   
``` 
set (CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-g")
```  
  
라고 하면 세미콜롬이 들어가서 죽는다.    
```
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
```  
로 하면 OK.  
      
타겟 이름을 libhoge로 해서 라이브러리를 빌드하면 liblibhoge.so 로 된다  
빌드 후에 파일의 복사하는 것이 아닌 set_target_property()를 사용하여 OUTPUT_NAME 속성을 변경하면 좋다    
단 선두의 "lib"만을 제거하는 것은 더 간단하므로 PREFIX 속성을 조작하면 삭제 OK.  
```
set (TARGET_NAME libhoge)
add_library (${TARGET_NAME} SHARED hoge.c hoge2.c)
set_target_properties (${TARGET_NAME}
                       PROPERTIES PREFIX
                       "")
```
  
  
  
## 번역 글 
- [빌드 옵션](build_option.md)
- [손쉬운 xxx-config.cmake을 만드는 방법](xxx-config.cmake.md)
- [IMPORTED 타겟](IMPORTED_target.md)
- [빌드 시에 공개 헤더 파일을 한 장소에 복사 하고 싶다](headerFile_copy.md)
- [find_package 와 pkg_check_modules에 의한 라이브러리 탐색](find_package_And_pkg_check_modules.md)
- [CMake에서 빌드하는 코드에 clang-tidy를 실행](clang-tidy.md)
- [조금씩 설정을 바꾸어서 같은 소스 코드를 빌드하고 싶을 때](change_config.md)
- [설정 파일 만들기](make_config.md)
- [cmake에서 빌드 시에 쉘 스크립트를 실행하기](build_shellscript.md )
- [라이브러리를 자동적으로 찾는 Find<package>.cmake 템플릿](find_package_templete.md )
- [라이브러리를 자동으로 다운로드 및 빌드](auto_lib_download.md)
- [Boost 라이브러리를 정적 링크해서 사용하기](boost_static_lib.md )  
- [CMake로 소스 파일 전달을 위해 압축 파일로 만들기](cmd_targz.md)
  
  
  
## 예제 코드  
- [cmake-tutorial](https://github.com/pyk/cmake-tutorial )
- [cmake-examples](https://github.com/ttroy50/cmake-examples )
- [cmake_cookbook](https://github.com/mechazoidal/cmake_cookbook )  
- [dev-cafe/cmake-cookbook](https://github.com/dev-cafe/cmake-cookbook )
    - ZeroMQ, minizip 등의 외부 라이브러리를 사용하는 예제 코드가 있음.
- [Linking GLEW with CMake](https://stackoverflow.com/questions/27472813/linking-glew-with-cmake )  
- [cross platform cmake project template](https://github.com/nekko1119/CMakeProjectTemplate ) 
- [(일어)CMake에서  멀티플랫폼 개발(라즈베리 파이용 크로스 컴파일 포함)](https://qiita.com/take-iwiw/items/50a47dd3ff3e9163de0d )
- [(일어)CMake에서 CUDA 애플리케이션 개발](https://qiita.com/take-iwiw/items/a29f017ea05fec77f8e1 )
- [(일어)CMake에서 OpenCV 애플리케이션 개발](https://qiita.com/take-iwiw/items/337dce2148b3d6cee560 )
- [(일어)c++14 with openmp + gtest-1.8의 cmake 환경을 준비](https://qiita.com/takeshi-uchitane/items/04f558cb405245cfabea )
  
  
  
## 외부 글
- [awesome-cmake](https://github.com/onqtam/awesome-cmake )
- [CMake 할때 쪼오오금 도움이 되는 문서](https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c )
- [자동으로 컴파일 대상 파일 리스트 만들기](https://kukuta.tistory.com/244 )
- [C++ Korea 제7회 세미나  - 현실적인(Practical) CMake]( https://github.com/CppKorea/CppKoreaSeminar7th/tree/master/2%20-%20%ED%98%84%EC%8B%A4%EC%A0%81%EC%9D%B8(Practical)%20CMake )
- [CMakeLists.txt 명령](https://sonseungha.tistory.com/372 )
- [1. CMake 소개와 예제, 내부 동작 원리](https://www.tuwlab.com/ece/27234 )
- [2. CMakeLists.txt 주요 명령과 변수 정리](https://www.tuwlab.com/ece/27260 )
- [3. CMakeLists.txt 기본 패턴](https://www.tuwlab.com/ece/27270 )
- [cmake에서 정적 라이브러리를 하나로 합치기](http://ospace.tistory.com/539 )
- [Bootstrapping a vcpkg-based cmake project in Visual Studio]( http://cpptruths.blogspot.com/2019/03/bootstrapping-vcpkg-based-cmake-project.html )
- [Bootstrapping a vcpkg-based project in Linux and Windows with idiomatic cmake]( http://cpptruths.blogspot.com/2019/03/bootstrapping-vcpkg-based-cmake-project_31.html )
- [(일어)CMake Advent Calendar 2014](https://qiita.com/advent-calendar/2014/cmake )
- [(일어)CMake: 프리컴파일드 헤더 작성과 이용](https://qiita.com/mrk_21/items/264f6135679239ff018a )
- [(일어)ccache](http://qiita.com/tags/ccache/items )
- [(일어) CMake로 빌드 후에 파일을 복사하는 방법](https://qiita.com/ykasuga/items/71b190825b2c434d4b05 )
- [(일어) CMake에서 자동 생성 파일을 다루는 방법](https://qiita.com/hotwatermorning/items/aceef2f56a7dcf33d78c )
  
  
## 라이브러리, 툴
- [cmake-conan](https://github.com/conan-io/cmake-conan )  
