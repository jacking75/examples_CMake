# CMake에서 빌드하는 코드에 clang-tidy를 실행
출처: https://qiita.com/yoyomion/items/8ff1f5a63b4b4f757732  
  
clang-tidy는 C++를 위해 좋은 코드로 자동 수정을 해주는 Linter이다.   
CMake를 사용하여 쉽게 수행하는 방법을 기록한다.  
  
  
## 확인 환경
Ubuntu16.04 (를 VirtualBox에서 돌리는 것)   
  
  
## 준비
다음이 필요한지 불 필요한지 잘 기억나지 않지만 필요할 수 있을지도.  
clang, clang-tidy 설치  
```
sudo apt install clang clang-tidy
```
  
  
## CMake 업데이트 (3.6 이상으로 한다)
cmake3.6 이상이면, cmake 자신이 clang-tidy에 대응하고 있기 때문에 쉽게 도입 할 수 있다.   
최신 cmake를 가져온다 (tar.gz) https://cmake.org/download/  
  
예를 들어, 아래와 같이 하면 할 수 있다.  
CMake를 다운로드하고 경로를 등록한다.  
[How to install latest cmake version in Linux / Ubuntu from command line](https://anglehit.com/how-to-install-the-latest-version-of-cmake-via-command-line/ )  
  
  
  
## CMake를 사용하여 clang-tidy 하기
-DCMAKE_CXX_CLANG_TIDY를 사용하여 뭔가 적당히 하면 잘 된다.  
  
### 환경의 예
Direcroty 구성  
<pre>
.
└── Qiita
    ├── CMakeLists.txt
    └── main.cpp
</pre>
  
CMakeLists.txt  
```
cmake_minimum_required(VERSION 3.5)
 
set(CMAKE_CXX_FLAGS "-std=c++14")
 
set(SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    )
 
add_executable(Hello ${SOURCE_FILES})
```
  
main.cpp  
```
#include <iostream>
 
int main(){
    std::cout << "Hello World!" << std::endl;
 
    float f = 0.0F;
    int i = (int)f;
    std::cout << i << std::endl;
}
```
  
  
### 빌드
clang-tidy 대해서 빌드하기  
<pre>
mkdir build
cd build
cmake -DCMAKE_CXX_CLANG_TIDY="clang-tidy;-checks=-*,google-readability-casting;-fix;-fix-errors;-header-filter=/home/yoyomion/Qiita" ../Qiita
make
</pre>
  
  
### 확인
열어 보면 지적된 부분이 자동으로 수정되어 있다.   
C++ 스타일의 캐스트가 있다.  
```
Qiita/main.cpp (clang-tidy 실행 후)
```
  
  
설명  
선두에 clang-tidy 실행 명령을 작성하고 이후는 세미콜론으로 구분으로 clang-tidy 용 옵션을 나열한다.  
| 書いてあること                        | 説明                                                                                                                                                                             |
|---------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| clang-tidy                            | clang-tidy 실행 명령. cmake 용.                                                                                                                                                |
| -checks=-*,google-readability-casting | clang-tidy 옵션.이번에는 -* 로 모든 검사기를 해제 한 후 google-readability-casting 검사기를 지정하고 있다.                                                       |
| -fix                                  | clang-tidy 옵션. 수정 제안을 자동 적용한다.                                                                                                                                 |
| -fix-errors                           | clang-tidy 옵션. 빌드 오류가 되었다고 해도 수정 제안을 적용한다.                                                                                                         |
| -header-filter=/home/yoyomion/Qiita   | clang-tidy 옵션. 사용자가 제공하는 헤더 파일이 있는 곳을 절대 경로 지정한다. 이 디렉토리에 있는 헤더는 시스템 헤더로 간주 되지 않고, clang-tidy으로 지적 받을 수있다. |
  
  