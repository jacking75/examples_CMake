# CMake 예제

## 예제 소개
- 예제 코드는 CMake_example 디렉토리에 있다.  


## 01 helloworld
간단한 1개 파일의 C++ 코드  

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
  
  
  
## 02 helloworld
컴파일러 옵션을 설정. -Wall, C++14  

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
  
  

## 03 helloworld
main 코드 파일 이외의 코드 파일이 있는 경우  

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
  
  

## 04 helloworld
mai.cpp 이외의 파일 다른 디렉토리에 있는 경우  

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
  
  

## 05 helloworld
static 파일을 만든 후 참조  

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
  
  

## 06 helloworld
컴파일러 지정  

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
  
  

## 07 helloworld
외부 라이브러리(여기서는 Boost 라이브러리) 사용하기  

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


## 컴파일러 링커에 대한 기본적인 설정
출처: https://www.wagavulin.jp/entry/2011/11/27/222642
  
## 소스 파일을 증가
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
  
## 인클루드 패스를 지정
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
  
  
## 매크로 지정
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
   
   
## 라이브러리 추가
링크 라이브러리를 추가 할 때 target_link_libraries 이다. 결과는 -l 옵션에 반영된다.
```
cmake_minimum_required (VERSION 2.8)
add_executable (myapp main.cpp)
target_link_libraries (myapp foo)
```  
라이브러리 이름뿐만 아니라 대상 이름도 지정해야 한다. 따라서 target_link_libraries는 add_executable 이후에 있어야 하며 먼저 지정하면 에러가 된다.

## 라이브러리 경로 지정
링크하는 라이브러리가있는 경로를 지정하려면 link_directories 명령을 사용한다.
```
cmake_minimum_required (VERSION 2.8)
link_directories (/path/to/lib)
add_executable (myapp main.cpp)
```
  
물론, 결과는 -L 옵션에 반영된다. 주의 사항으로는 link_directories 이때까지 이미 생성된 대상만 효과가 있다는 것이다. 

앞의 예에서 말하면, link_directories 행과 add_executable의 행을 바꾸면 무효가 된다. 그리고 target_link_libraries의 경우와 달리, 먼저 배치하면 오류 경고가 나오지  않는다.

덧붙여서 include_directories과 add_definitions은 어디에 두어도 유효된다.

## 정리
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
    
	
## 외부 글
- [CMake 할때 쪼오오금 도움이 되는 문서](https://gist.github.com/luncliff/6e2d4eb7ca29a0afd5b592f72b80cb5c )

