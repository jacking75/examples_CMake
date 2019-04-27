# IMPORTED 타겟 - cmake
출처: https://qiita.com/osamu0329/items/3e9868e83d3745b8c8b6  
   
## 시작하기
CMake 프로젝트 이외에서 작성된 기존 라이브러리와 독점 라이브러리로 소스 코드가 공개 되지 않은 라이브러리를 CMake 프로젝트에서 이용 하고 싶은 경우가 있다.  
  
이러한 라이브러리를 CMake의 대상으로 취급하기 위한 구조가 IMPORTED 타겟이다.  
(라이브러리뿐만 아니라 실행 파일을 IMPORTED 타겟으로 정의 할 수 있다)
  
이 밖에도 package-config.cmake를 작성할 때 설치 트리의 라이브러리 파일을 IMPORTED 대상으로 정의하는 용도도 있다. 
사실, install (EXPORT) 명령이 생성하는 export.cmake는 IMPORTED 타겟을 사용하고 있다.  
  
  
  
## IMPORTED 라이브러리를 정의한다
add_library() 명령에 IMPORTED 옵션을 지정하면 대상을 IMPORTED 대상으로 정의 할 수 있다. 또한, 일반적으로 의식 할 필요는 없지만 타겟 속성에 IMPORTED가 정의된다.  
```
add_library(foo SHARED IMPORTED )
```
  
정확한 구문은 아래와 같다.  
```
add_library(<name> <SHARED | STATIC | MODULE | UNKNOWN> IMPORTED [GLOBAL] )
```
  
IMPORTED 대상은 정의된 디렉토리나 그 아래의(즉 add_subdirectory) 디렉토리에서 찾을 수 있다. 상위 디렉토리에서 참조 할 수 없다.   
만약 상위 디렉토리에서 참조하려면, GLOBAL 옵션을 부여한다.  
  
  
  
## 라이브러리 파일의 위치를 지정한다( IMPORTED_LOCATION 속성)
add_library(<name> ... IMPORTED) 한 것만으로는 실제 라이브러리 파일의 위치를 CMake가 알 수 없기 때문에 사용자가 명시적으로 라이브러리 파일의 위치를 CMake에 알려야 한다.  
라이브러리 파일의 위치를 전달하려면 다음과 같이 IMPORTED_LOCATION 속성 (또는 IMPORTED_LOCATION_<CONFIG> 속성)을 설정한다.  
  
IMPORTED_LOCATION 설정  
```
set_target_properties (foo
    PROPERTIES
        IMPORTED_LOCATION /path/to/real/library/libfoo.so)
```
  
또한 DLL 환경에서 SHARED 라이브러리의 위치를 IMPORTED_LOCATION 에 지정하는 경우에는 .lib 파일이 아닌 .dll 파일을 지정한다.   
import 라이브러리( .lib) 파일의 위치는 IMPORTED_IMPLIB 또는 IMPORTED_IMPLIB_<CONFIG>로 지정한다.  
[IMPORTED_LOCATION 문서 (CMake 3.0)](http://www.cmake.org/cmake/help/v3.0/prop_tgt/IMPORTED_LOCATION.html#prop_tgt:IMPORTED_LOCATION  ) 
  
  
  
## SONAME을 지정한다( IMPORTED_SONAME)
IMPORTED_SONAME 또는 IMPORTED_SONAME_<config> 속성을 사용하면 SONAME을 지정할 수 있다.  
  
  
## 클라이언트 타겟이 사용해야 할 인클루드 디렉토리를 지정한다( INTERFACE_INCLUDE_DIRECTORIES)
IMPORTED 라이브러리를 사용하는 프로그램을 컴파일 할 때 소정의 인클루드 디렉토리에 존재하는 헤더 파일의 include를 요하는 경우가 있다.  
INTERFACE_INCLUDE_DIRECTORIES 속성을 지정하면 target_link_libraries(client foo)같은 foo 라이브러리를 링크 지정하여  client 대상에 target_include_directories에서 foo의 INTERFACE_INCLUDE_DIRECTORIES에서 지정한 디렉토리가 부여된다.  
```
set_target_properties(foo PROPERTIES INTERFACE_INCLUDE_DIRECTORIES /path/to/foo/headers)
   
# 다른 어딘가
add_executable(client client.cc)
target_link_libraries(client foo)
 
# 아래를 생략할 수 있다
# target_include_directories(client /path/to/foo/headers)
```
  
이처럼 INTERFACE_INCLUDE_DIRECTORIES 를 활용하면 /path/to/foo/headers를 변수 foo_INCLUDE_DIRECTORIES 로 설정하여, 개별 대상에 target_include_directories 명령을 부여하는 번거로운 작업을 생략 할 수 있다.  
  
  
  
## 클라이언트 대상이 사용해야 할 컴파일러 정의를 지정하는 INTERFACE_COMPILE_DEFINITIONS
이 라이브러리를 링크하는 실행 파일이나 다른 라이브러리를 컴파일 할 때 부여해야 할 컴파일러 정의를 지정한다.  
  
IMPORTED 대상 이외에도 사용할 수 있지만,이 경우 라이브러리 대상에 target_compile_definitions() 명령으로 지정할 때  PUBLIC 이나 INTERFACE 옵션을 부여하는 것으로,이 속성이 자동으로 설정된다.
  
  
## 클라이언트 대상이 사용해야 할 컴파일러 옵션을 지정한다 INTERFACE_COMPILE_OPTIONS
이 라이브러리를 링크하는 실행 파일이나 다른 라이브러리를 컴파일 할 때 부여해야 할 컴파일러 옵션을 지정한다. IMPORTED 대상 이외에도 사용할 수 있다.  
타겟이 IMPORTED 타겟이 아니면 target_compile_options 명령에 INTERFACE 또는 PUBLIC 옵션을 부여하여 자동으로 설정할 수 있다.  
  
  
## 의존 라이브러리를 지정한다 ( IMPORTED_LINK_DEPENDENT_LIBRARIES)
가져 오기 라이브러리 자체가 다른 라이브러리에 의존하고 있는 것이 있다.   
의존하는 라이브러리들을 IMPORTED_LINK_DEPENDENT_LIBRARIES 속성에 지정하여 가져온 라이브러리를 링크 할 때 이 속성에 지정된 라이브러리들을 동시에 연결할 수 있다.  
  
note  
자세한 내용은 쫓지 않았지만`INTERFACE_LINK_LIBRARIES`는 2.8.12에서 도입된 새로운 속성이 있고, 같은 동작을 한다. (INTERFACE_LINK_LIBRARIES로 대체된 IMPORTED_LINK_INTERFACE_LIBRARIES 라는 속성도 있다)  
 