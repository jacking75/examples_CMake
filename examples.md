## cmake 사용 예제
- 사용할 코드는 Cmake_example 디렉토리에 있다.



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
