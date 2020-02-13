출처: http://mklearning.blogspot.com/2020/01/cmakegoogle-test.html  
  
C++에서 단위 테스트를 할 때 후보로 Google Test를 들수 있다.  
공식 문서에는 CMake를 이용하여 기존의 프로젝트에 Google Test를 통합하는 방법이 적혀있지만, 정리 해 보았다.  
  
먼저 다음과 같은 구조를 가정한다.  
- CMakeLists.txt
    - src와 test를 add_subdirectory에 추가
- src(프로젝트 소스 코드)
- test
    - GoogleTest.CMakeLists.txt.in
    - CMakeLists.txt
- build (빌드용 폴더, 버전 관리 하지 않는)
  
GoogleTest.CMakeLists.txt.in의 내용은 문서에 있는 CMakeLists.txt.in과 같다.  
  
test 폴더의 CMakeLists.txt의 첫 번째 줄, configure_file를 해석하면 아래와 같은 상태가된다.  
- build
    - googletest-download
        - CMakeLists.txt
            
그리고 이 googletest-download 안의 CMakeLists.txt는 configure_file 실행 시의 CMAKE_CURRENT_BINARY_DIR로 해석 되므로 아래와 같은 내용이다.  
```
ExternalProject_Add (googletest
    ... 생략 ...
    SOURCE_DIR "build의 절대 경로/googletest-src"
    BINARY_DIR "build의 절대 경로/googletest-build"
)
```
  
그럼 execute_process에 의해 루트 CMakeLists.txt에 대한 빌드 설정을 생성 할 때 googletest-download의 CMakeLists.txt가 실행 되고 폴더가 아래와 같은 구조로 된다.  
- build
    - googletest-download
    - googletest-src
    - googletest-build
      
이제 googletest를 통합하는 준비가 끝났으므로, 이 후에 단위 테스트의 대상을 add_executable 하여 gtest_main와 연결하고, add_test 테스트에 등록할 뿐이다.  
