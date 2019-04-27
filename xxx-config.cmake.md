# 손쉬운 xxx-config.cmake을 만드는 방법
출처: https://qiita.com/osamu0329/items/134de918c0ffa7f0557b
  
  
## 동기
CMake로 라이브러리를 만들 때,  <foo>-config.cmake 파일을 생성해 두면, 이 라이브러리를 사용하는 CMake 프로젝트에서 find_package 명령을 이용하여 검색 할 수 있다.  
이 foo-config.cmake를 손으로 만들 수 있지만, 이것은 꽤 힘든 작업이다.  
이 글에서는 install (EXPORT) 명령을 사용하여 foo-config.cmake를 자동 생성하는 CMakeLists.txt 작성에 대해 설명한다.  
   
  
## 이 방법의 장점
FindFoo.cmake을 쓸 필요가 없다.  
클라이언트 측의 CMakeLists.txt에서 find_package(foo) 한 후에 target_link_libraries(foo) 만으로 자신만의 라이브러리를 링크 할 수 있다.  
    
  
## 대략적인 단계
자작 라이브러리를 install(TARGETS) 할 때  EXPORT <export-name> 옵션으로 내보내기 이름을 부여하고,  
install(EXPORT <export-name>) 명령으로 설치 트리를 위한  foo-config.cmake 파일을 생성하여 설치  
(옵션) export(TARGETS) 명령으로 빌드 트리용  foo-config.cmake 파일이 생성된다  
  
다음 샘플에서는 usage-requirement를 사용하여이 라이브러리를 find_package 하여 사용하는 CMake 프로젝트 target_link_link_libraries()를 실시하는 것만으로 다음을 할 수 있게 하고 있다. 
- 자동으로 포함 경로가 추가된다 
- 의존 라이브러리가 링크 대상에 포함 (다음 예에서는 foo를 링크하면 bar가 자동으로 링크된다)
  
  
## 실제 샘플 (설치 트리 용 샘플)
라이브러리 foo를 작성하여 라이브러리를 위한 foo-config.cmake를 자동으로 생성하는 샘플을 설명한다.   
CMakeLists.txt 아래와 같다.  
```
cmake_minimum_required(VERSION 2.8.12)
 
project(libfoo C)
 
# 1. INTERFACE_INCLUDE_DIRECTORIES 에 소스 디렉토리와 빌드 디렉토리를 자동으로 추가한다.
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
 
# 2. 라이브러리의 빌드 지정
add_library(foo SHARED foo.c)
 
# 3. 설치 할 헤더 파일을 지정해 둔다.
# 지정한 파일에는 INSTALL(TARGETS) 의 PUBLIC_HEADER 로 지정한 디렉토리에 설치된다.
set_target_properties(foo PROPERTIES
        PUBLIC_HEADER foo/foo.h
)
 
# 4. foo 라이브러리가 의존하는 라이브러리를 지정한다.(옵션)
# PUBLIC 지정하여 링크하면 의존 라이브러리가 export된다
target_link_libararies(foo PUBLIC bar)
 
# 5. 타겟의 설치 지정
install(TARGETS foo
    EXPORT foo-export                         # 5.a EXPORT 옵션으로 export 이름을 지정한다
    LIBRARY        DESTINATION lib            # 5.b 라이브러리 설치할 곳
    INCLUDES       DESTINATION include        # 5.c export 시에 include 패스에 포함하는 패스를 지정한다
    PUBLIC_HEADER  DESTINATION include/foo)   # 헤더 설치할 곳
 
# 6. foo-config.cmake 을 자동 생성하여 설치한다
install(EXPORT foo-export                     # 6.a
    FILE foo-config.cmake                     # 6.b 파일 이름을 지정한다
    DESTINATION share/cmake/foo/              # 6.c 설치할 곳
    EXPORT_LINK_INTERFACE_LIBRARIES)          # 6.d 동시에 링크해야할 라이브러리를 export 한다
```
  
위의 프로젝트를 빌드하고 설치하면 ${CMAKE_INSTALL_PREFIX}/share/cmake/foo/foo-config.cmake 가 생성된다.  
생성된 foo-config.cmake 에서는 다음이 이루어진다.   
1. 대상 foo를 IMPORTED 라이브러리로 정의한다.   
2. 대상 foo의 위치 (IMPORTED_LOCATION)에 설치된 라이브러리 파일을 지정한다. 
3. 대상 foo의 INTERFACE_INCLUDE_DIRECTORIES 속성에 설치할 foo / include 디렉토리를 지정한다.
  
1, 2는 find_package (foo) 프로젝트에서 타겟 foo를 사용하여 링크 할 수 있다.   
3에 따라서 target_link_libraries(foo) 하여 foo/include 가 자동으로 포함된다.  
  
위의 foo 프로젝트의 CMakeLists.txt 에 아래의 export (EXPORT) 명령을 추가하면 foo 빌드 트리에서 foo 타겟을 사용하는 foo-export.cmake 파일이 foo 프로젝트의 빌드 트리에 생성된다. 
이 파일은 foo 프로젝트를 설치하지 않고도 사용할 수 있다.  
  
### 상세  
CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE 을 유효하게 한다.   
이 CMake 변수를 ON으로 설정하면 타겟의 INTERFACE_INCLUDE_DIRECTORIES 에 소스 디렉토리와 빌드 디렉토리가 자동으로 추가된다.  
  
라이브러리의 빌드 지정한다. 보통의 지정과 같음로 상관하지 않는다.  

### 설치 헤더 지정  
라이브러리 링크 지정  

타겟 설치 지정  
평소와 마찬가지로 foo 타겟의 설치할 곳을 지정하지만 이 때 몇 가지 추가 옵션을 지정한다.   
또한 지정하는 경로가 상대 경로의 경우 CMAKE_INSTALL_PREFIX 에서의 상대 경로이다.  
a. 후속의 install (EXPORT)에서 이용하는 내보내기 이름을 지정한다. 여기에서는 foo-export 라는 이름을 지정했다.  
c. 생성되는 foo-config.cmake 에서 foo 타겟에 지정하는 INTERFACE_INCLUDE_DIRECTORIES 속성의 설정 값을 지정한다. 이 때 5.d 에서 지정한 헤더를 설치할 일관성을 갖게한다.  
  
install(EXPORT) 명령에서 foo-config.make의 자동 생성 및 설치 설정을 한다.  
a. foo-export 에는 5에서 지정한 내보내기 이름을 지정한다.  
b. 자동 생성하는 파일 이름을 지정한다.  
c. 설치 위치를 지정한다. find_package 검색 경로를 지정하면 편리하다.  
d. foo 라이브러리 LINK_INTERFACE_LIBRARIES을 지정한다.  
  
  
## 설치하지 않고, foo 타겟을 이용하고 싶다
빌드 트리에서 직접 foo 타겟을 사용하기 위해 foo-export.cmake 을 생성하기 위해서는 아래의 export(EXPORT) 명령을 foo 프로젝트에 추가한다.  
빌드 트리를 위한 추가 명령  
```
# 빌드 트리에서 타겟을 IMPORT 하기 위해 CMake 파일을 생성한다(옵션)
export(EXPORT foo-export FILE foo-export.cmake
       EXPORT_LINK_INTERFACE_LIBRARIES)
```
  
생성된 foo-export.cmake 에서는 빌드 트리 내의 라이브러리 파일과 디렉토리를 참조한 IMPORTED  타겟이 정의된다.
  
  
  
## 라이브러리 이용 측의 프로젝트
장점에서 언급한 바와 같이 find_package(foo) 를 실행하면 foo-config.cmake가 검색 되고, 그 중 foo 라는 IMPORTED 대상이 정의된다. 
라이브러리 이용 측의 프로젝트에서는 target_link_libraries(<target> foo) 라고 하면 자동으로 include_directory 와 의존 라이브러리의 링크가 실시된다.  
  
아래에 foo 라이브러리를 사용하는 프로젝트의 CMakeLists.txt 의 샘플을 기재한다.  
```
cmake_minimum_required(VERSION 2.8.12)
 
project(foo-client C)
 
find_package(foo REQUIRED)
 
add_executable(foo-client foo-client.c) # foo 라이브러리를 사용하는 실행 파일을 만든다
 
target_link_libraries(foo-client foo)   # foo 라이브러리를 링크한다.
                                        # 자동적으로 include 디렉토리도 설정되기 때문에 target_include_directories 을 기술하지 않아도 된다.
```
  
주의: 다단의 의존 관계 문제  
참조 http://www.itk.org/Bug/print_bug_page.php?bug_id=1101  
  
다단의 의존 관계가 존재하면 라이브러리 경로가 잘 계승 할 수 없는 것 같다.   
예를 들어 다음과 같은 세 가지 CMake 프로젝트가 있다고 한다.  
foo : 라이브러리  
bar : foo 라이브러리에 의존하는 라이브러리  
client : bar에 의존하는 실행 프로그램  
  
client를 빌드 할 때 foo 라이브러리의 설치처가 인계되지 않아서, 링크 오류가 발생할 수 있다.  
이를 해소하기 위해 클라이언트에서 명시 적으로 find_package(foo) 하던가, 혹은 bar-config.cmake 에  find_package(foo)를 기술 할 필요가 있는 것 같다.  

