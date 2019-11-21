# 라이브러리를 자동으로 다운로드 및 빌드
실행 파일을 빌드하기 위해 기본적으로 설치 되지 않은 라이브러리가 필요하다.    
CMake는 빌드에 필요한 라이브러리를 자동으로 다운로드 하고 빌드하는 ExternalProject_Add 기능이 있다.    
  

## ExternalProject_Add 함수의 기본
  
```
ExternalProject_Add ( <target name> <option> ...] )
```


## GitHub 등에서 다운로드하여 라이브러리를 빌드하는 경우
예를 들어 GoogleTest를 자동으로 다운로드 및 빌드 하고자 하면 이런 식으로 작성한다.    
```
include ( ExternalProject )
 
# GoogleTest을 빌드 할 디렉토리를 지정
# (여기에서는 빌드 디렉토리의 gtest)
set ( GTEST_BUILD_DIR $ { CMAKE_BINARY_DIR } / gtest )
# GoogleTest를 설치할 디렉토리를 지정
# (여기에서는 프로젝트 루트 디렉터리에 gtest)
set ( GTEST_INSTALL_DIR $ { CMAKE_SOURCE_DIR } / gtest )
# GoogleTest 헤더 파일이 있는 디렉토리
set ( GTEST_INCLUDE_DIR $ { GTEST_INSTALL_DIR } / include )
# GoogleTest 정적 라이브러리가 있는 폴더
set ( GTEST_LIB_DIR $ { GTEST_INSTALL_DIR } / lib )
 
# GoogleTest는 pthread가 필요하므로 CMake에게 찾고 달라고
find_package ( Threads REQUIRED )
 
# GoogleTest를 외부 프로젝트로 추가
ExternalProject_Add ( googletest
         # 다운로드 할 URL을 지정
        URL https://github.com/google/googletest/archive/release-1.8.0.tar.gz
        # GoogleTest을 빌드 할 디렉토리를 지정
        PREFIX $ { GTEST_BUILD_DIR }
        # GoogleTest을 다운로드하여 압축 한 후 cmake를 실행할 때 전달되는 인수
        #이 경우 스스로`cmake -DCMAKE_INSTALL_PREFIX = ...`라고 하는 것과 같다
        # cmake에서 원래 정의된 변수 (CMAKE_CXX_FLAGS) 및 라이브러리에서 정의 되어 있는
        # 변수를 사용 할 수 있다. 인수의 머리에 `-D`을 달아주는 것을 잊지 말자!
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX : PATH = $ { GTEST_INSTALL_DIR }
    )
 
# GoogleTest에 gtest라는 이름을 붙여 외부 (=이 프로젝트에서 빌드 방법을 정의 하지 않는다)
# 정적 라이브러리로 등록
add_library ( gtest IMPORTED STATIC GLOBAL )
# gtest에 대응하는 정적 라이브러리 파일과 종속 라이브러리 pthread 를 지정
set_target_properties ( gtest PROPERTIES
    IMPORTED_LOCATION $ { GTEST_LIB_DIR } /libgtest.a
    IMPORTED_LINK_INTERFACE_LIBRARIES $ { CMAKE_THREAD_LIBS_INIT }
)
 
# gtest_main도 필요한 경우 add_library에서 set_target_properties까지
# gtest을 gtest_main에 다시 추가하기
  
각각의 라이브러리에서 직접 지정하는 변수를 알고 싶으면 각각의 라이브러리의 루트 디렉토리에서 cmake -L 라고하면 나타난다. 
   
add_library(gtest ...) 로 하는 것으로, 실행 파일에 쉽게 연결할 수 있다 : 
 
add_executable ( <target> <sources> )
target_link_libraries ( <target> gtest )
```
  
공유 라이브러리의 경우 STATIC를 SHARED로 바꾸어 주자.     
또한 GTEST_BUILD_DIR 와 GTEST_INSTALL_DIR를 캐시 변수로 하면 cmake -L 할 때나 ccmake를 사용하여 configure 할 때 변수가 나타날 수 있다 :    
```
set ( GTEST_INSTALL_DIR $ { CMAKE_SOURCE_DIR } / gtest CACHE PATH "GoogleTest install directory" )
```
    
      
## 다운로드 하지만 빌드 등은 필요없는 경우 (헤더 만의 라이브러리)
C++ 템플릿을 사용하여 헤더 만의 라이브러리가 있다.
- [Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
- [pybind11](https://github.com/pybind/pybind11)  
    
이러한 라이브러리의 경우 다운로드 한 후 빌드하지 않고 헤더 파일만 특정 디렉토리에 복사하고 싶으므로 다음과 같이 쓸 수 있다.    
```
# Eigen 빌드 설치 포함 디렉터리를 정의
set ( EIGEN_BUILD_DIR    $ { CMAKE_BINARY_DIR } / eigen )
set ( EIGEN_INSTALL_DIR $ { CMAKE_SOURCE_DIR } / eigen )
set ( EIGEN_INCLUDE_DIR $ { EIGEN_INSTALL_DIR } / include )
 
# Eigen을 외부 프로젝트로 추가
ExternalProject_Add (
    eigen
    URL http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz
    PREFIX $ { EIGEN_BUILD_DIR }
    # CMake의 configure는 필요 없기 때문에 공백 ("")를 제공함으로써 아무것도 시키지 않는다
    CONFIGURE_COMMAND ""
    # 빌드도 불필요
    BUILD_COMMAND ""
    # 설치도 불필요
    INSTALL_COMMAND ""
    # 테스트도 불필요
    TEST_COMMAND ""
    )
 
# Eigen을 다운로드 하여 압츌울 풀 수 있는 폴더를 얻는다
ExternalProject_Get_Property ( eigen SOURCE_DIR )
 
# 위에서 얻은 폴더내의 지정 디렉토리의 내용을 다른 폴더에 복사
# 이 경우 $ {SOURCE_DIR} / Eigen -> $ {EIGEN_INCLUDE_DIR} / Eigen로 복사된다
ExternalProject_Add_Step (
    # 대상의 외부 프로젝트
    eigen
    # 수행 할 작업
    copySource
    # 대상 디렉토리
    WORKING_DIRECTORY $ { SOURCE_DIR }
    # 명령
    COMMAND $ { CMAKE_COMMAND } -E copy_directory Eigen $ { EIGEN_INCLUDE_DIR } / Eigen
    COMMENT "Copying headers to $ { EIGEN_INCLUDE_DIR } / Eigen"
    )
 
# 다음은 인클루드 디렉토리로 등록하는 것만
include_directories ( SYSTEM $ { EIGEN_INCLUDE_DIR } )
```
  