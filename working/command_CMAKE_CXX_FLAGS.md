# CMAKE_CXX_FLAGS이에 대응하는 CMake 명령
  
set에서 다음과 같이 정리해서 옵션을 줄 수 있다.

 set( CMAKE_CXX_FLAGS "-O2 -std=c++11 -Wall ..." )


(CMake의 변수는 [CMake useful variables](https://cmake.org/Wiki/CMake_Useful_Variables)를 참조하는 것이 가장 빠르다)  
  
자, 그럼 이 경우 set 명령을 어디에 두어야할까? 이것을 결정하려면 CMake에서 변수의 범위를 알 필요가 있다.   
부모 디렉토리에 정의되어 있으면 하위 디렉토리에서도 사용할 수 있다.  
최적화 등의 옵션은 빌드 전반에 걸쳐 동일한 것을 사용하기 때문에 프로젝트 이름을 정의한 후 서브 디렉토리를 등록하기 전에 두면 하위 디렉토리에서의 처리에도 같은 옵션이 적용된다.  
  
실제로 다양한 위치를 바꾸어 빌드하고, make VERBOSE=1 로 빌드 세부 사항을 토해 내게 해 보았지만, set을 둔 이후로 밖에 사용되지 않았기 때문에 해당 옵션이 필요한 처리 전에 오도록 배치해야 한다.  


/CMakeLists.txt
cmake_minimum_required ( VERSION 2.8 ) 
project ( test_cmake CXX ) 
set ( CMAKE_CXX_FLAGS "-O2 -std = c ++ 11 -Wall" ) 
add_subdirectory ( src ) 
add_subdirectory ( test )

  
## CXX_STANDARD
CMake 3.1에서 std 옵션을 CMAKE_CXX_STANDARD을 사용하여 지정할 수 있게 되었다.   
  
 
/CMakeLists.txt
cmake_minimum_required ( VERSION 3.1 ) 
project ( test_cmake CXX ) 
set ( CMAKE_CXX_STANDARD 11 ) 
set ( CMAKE_CXX_FLAGS "-O2 -Wall" ) 
add_subdirectory ( src ) 
add_subdirectory ( test )

  
## 빌드의 종류에 따라 옵션을 변경
디버그 모드로 빌드하고 싶을 때 등 일일이 CMakeLists.txt의 내용을 변경하는 것은 번거롭다.  
그래서 CMake는 미리 빌드의 종류에 따라 별도의 옵션을 정의 해두고 빌드 할 때 지정할 수 있도록 되어 있다.  
  
 
/CMakeLists.txt
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

로 하면 CMake를 실행할 때 -DCMAKE_BUILD_TYPE을 지정 해주면 지정된 모드의 플래그를 사용하여 빌드 해준다.  

> cmake ..                                     # 기본 
> cmake -DCMAKE_BUILD_TYPE = Debug ..            # 디버그 
> cmake -DCMAKE_BUILD_TYPE = Release ..          # 자료 
> cmake -DCMAKE_BUILD_TYPE = MinSizeRel ..       # 최소 크기 자료 
> cmake -DCMAKE_BUILD_TYPE = RelWithDebInfo ..   # 디버깅 정보를 더 해서  출시

 
각 파일을 컴파일 할 때 CMAKE_CXX_FLAGS 뒤에 각 모드의 플래그가 추가된다.  
여러 충돌 하는 옵션이 있을 경우 나중의 옵션이 채용된다.  
또한 코드의 프로파일을 실시하는 등 일시적으로 옵션을 추가하려면 CMake를 실행할 때 프래그를 전달하여 추가 할 수 있다.  

> cmake -DCMAKE_CXX_FLAGS = "-pg" ..

그러나 이렇게 하려면 플래그 작성을 다음과 같이 해야한다.  

/CMakeLists.txt
set ( CMAKE_CXX_FLAGS " $ { CMAKE_CXX_FLAGS } -O1 -Wall" )

이렇게하면 명령 줄에서 주어진 옵션이 추가 될 수 있다.

g++의 기본 설정
- Default :
- Debug : -g
- Release : -O3 -DNDEBUG
- MinSizeRel : -Os -DNDEBUG
- RelWithDebInfo : -O2 -g -DNDEBUG  