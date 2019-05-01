# 라이브러리를 자동으로 찾는 Find<package>.cmake 템플릿
출처: https://qiita.com/shohirose/items/d9bda00a39a113965c5c
  
  
## 서두
CMake에는 자신의 프로젝트에 속해 있지 않는 라이브러리를 자동적으로 찾ㄱ아주는 편리한 명령어 'find_package'가 있다.  
예를 들면 Boost 라이브러리를 자작 프로그램에서 사용하고 있는 경우,  
```
cmake_minimum_required(VERSION 3.8.2)
project(find_package_example CXX)
find_package(Boost REQUIRED)
add_executable(foo foo.cpp)
target_link_libraries(foo
  Boost::boost
  )
```
  
로 하면 'foo'를 컴파일 할 때 Boost 라이브러리의 헤더 파일이 인크루드 된다.    
  
아주 편리하지만 모든 라이브러리에 대해서 'find_package'을 사용할 수 있지는 않다.  
이 명령어를 사용할 때는 자작 라이브러리를 검색하는 로직을 가리키는 'Find<package>.cmake' 또는 '<package>Config.cmake '/ '<lower-case-package>-config.cmake'가 있어야 한다.  
  
기본적으로 C/C++ 라이브러리를 만든는 측이 이들 스크립트를 제공해야 한다.  
CMake가 <package>-config.cmake를 자동 생성하는 기능을 제공하고 있으므로 이쪽을 사용한다.  
  
단 제공 되지 않은 경우는 자작할 필요가 있다.  
여기에 필요 최소한의 요소를 갖춘 'Find<package>.cmake' 템플릿을 올리므로 참고하기 바란다.  
보다 자세한 설정을 하고 싶은 경우는 (플랫폼 이나 버전을 참고 하는 등)은 CMake가 제공하는 스크립트(예를 들면 FindPNG.cmake)를 참고하기 바란다.  
'Find<package>.cmake'를 만들고 싶다면 'CMAKE_MODULE_PATH'에 이 스크립트를 놓아 두고 템플릿을 추가한다.  
```
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} <path-to-dir>)
```
  
이것만으로 'find_package(<package>)'를 사용할 수 있도록 된다.  
예를 들면 아래의 예의 스크립트를 사용하면  
```
# 프로젝트의 루트 디렉토리 아래에 있는 cmake 라는 디렉토리에 
# FindGMP.cmake을 두고, 이 디렉토리를 등록
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

# 라이브러리를 검색
find_package(GMP REQUIRED)

add_executable(myapp main.cpp)
# myapp 에 GMP를 링크
target_link_libraries(myapp GMP::GMP)
```
  
로 할 수 있도록 된다.   
  

자작한 'Find<Package>.cmake'를 [github](https://github.com/shohirose/cmake_find_package )에 업로드 했으므로 자유롭게 이용하기 바란다.  현재 이용가능한 패키지는 아래 항목이다.  
- CGAL(The Computational Geometry Algorithms Library)
- GMP(The GNU Multiple Precision Arithmetic Library)
- MPFR(The GNU MPFR Library)
- Microsoft GSL
- Spdlog
  
  
  
## 템플릿
필요 최소한의 명령어가 되는 'Find<package>.cmake'는 아래와 같다.  
```
find_path(<package>_INCLUDE_DIR ...)
find_library(<package>_LIBRARY ...) # 헤더만의 라이브러리의 경우는 불 필요
mark_as_advanced(
  <package>_INCLUDE_DIR
  <package>_LIBRARY     # 헤더만의 라이브러리의 경우는 불 필요
  )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(<package>
  REQUIRED_VARS
    <package>_INCLUDE_DIR
    <package>_LIBRARY      # 헤더만의 라이브러리의 경우는 불 필요
  )

if(<package>_FOUND AND NOT TARGET <package>::<package>)
  add_library(<package>::<package> UNKNOWN IMPORTED)
  set_target_properties(<package>::<package> PROPERTIES
    IMPORTED_LINK_INTERFACE_LANGUAGES ["C"|"CXX"]  # 헤더만의 라이브러리의 경우는 불 필요
    IMPORTED_LOCATION "${<package>_LIBRARY}"       # 헤더만의 라이브러리의 경우는 불 필요
    INTERFACE_INCLUDE_DIRECTORIES "${<package>_INCLUDE_DIR}"
    )
endif()
```
  
아래의 예에서는 UNIX만 고려하고 있다.  
  
  
  
## 컴파일된 라이브러리
예를 들면 [GMP 라이브러리](https://gmplib.org/ )를 검색하는 스크립트는 아래처럼이다.  
  
FindGMP.cmake  
```
# 인크루드 디렉토리의 패스를 gmp.h를 토대로 검색한다
find_path(GMP_INCLUDE_DIR gmp.h
  # 검색할 패스
  PATHS
    # 환경 변수 GMP_ROOT 또는 GMP_INCLUDE_DIR가 존해한다면 그기를 검색
    ENV GMP_ROOT
    ENV GMP_INCLUDE_DIR
    # CMake의 변수로서 GMP_ROOT를 정의 되어 있다면 그기를 검색
    ${GMP_ROOT}
    /usr
    /usr/local
  PATH_SUFFIXES
    include
  )
# 라이브러리 패스를 라이브러리 이름을 토대로 검색한다
find_library(GMP_LIBRARY
  NAMES
    gmp
  PATHS
    ENV GMP_ROOT
    ENV GMP_LIB_DIR
    ${GMP_ROOT}
    /usr
    /usr/local
  PATH_SUFFIXES
    lib
  )
# advanced 모드가 아닌한 변수의 존재를 찾는다
mark_as_advanced(GMP_INCLUDE_DIR GMP_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMP
  REQUIRED_VARS
    GMP_INCLUDE_DIR
    GMP_LIBRARY
  )

# GMP를 발견하지만 GMP::GMP가 정의 되어 있지 않은 경우
if(GMP_FOUND AND NOT TARGET GMP::GMP)
  # GMP::GMP라는 타겟 이름으로 GMP 라이브러리를 정의
  # UNKNOWN = STATIC/SHARED 인지는 아직 불명
  # IMPORTED = 이 프로젝트를 속하지 않은 타겟
  add_library(GMP::GMP UNKNOWN IMPORTED)
  set_target_properties(GMP::GMP PROPERTIES
    # C언어, C++ 이라면 "CXX"라고 한다
    IMPORTED_LINK_INTERFACE_LANGUAGES "C"
    IMPORTED_LOCATION "${GMP_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${GMP_INCLUDE_DIR}"
    )
endif()
```
  
  
  
## 헤더만의 라이브러리 
예를 들면 Eigen 라이브러리를 찾을 때는 스크립트는 아래처럼한다.  
(CMake를 사용하여 Eigen 라이브러리를 인크루드 하면 Eigen3Config.cmake가 자동 생성되므로 사실 만들 필요는 없다.)  
```
FindEigen.cmake
find_path(EIGEN_INCLUDE_DIR Eigen/Core
  PATHS
    ENV EIGEN_ROOT
    ENV EIGEN_INCLUDE_DIR
    ${EIGEN_ROOT}
    /usr
    /usr/local
  PATH_SUFFIXES
    include
  )
mark_as_advanced(EIGEN_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Eigen
  REQUIRED_VARS EIGEN_INCLUDE_DIR
  )

if(Eigen_FOUND AND NOT TARGET Eigen::Eigen)
  # Eigen은 헤더만의 라이브러리이므로 INTERFACE 키워드를 지정한다
  add_library(Eigen::Eigen INTERFACE IMPORTED)
  set_target_properties(Eigen::Eigen PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${EIGEN_INCLUDE_DIR}"
    )
  # set_target_properties 대신에 target_include_directories를 사용해도 OK
  # target_include_directories(Eigen::Eigen INTERFACE ${EIGEN_INCLUDE_DIR})
endif()
```
  