# find_package 와 pkg_check_modules에 의한 라이브러리 탐색
출처: https://www.wagavulin.jp/entry/2017/02/20/082608
    
  
외부 의존 라이브러리가 있는 C/C++ 코드를 CMake에서 빌드하는 경우 포함 경로와 라이브러리 경로를 지정할 필요가 있다.   
시스템에 설치된 라이브러리를 사용하게 된다면 이 경로를 찾아 지정해야 한다. 시스템에 설치된 장소가 다를 수 있기 때문이다.   
Unix 계열 OS에서 자주 사용되는 GNU Autotools 환경에서는 라이브러리의 검색이 Autoconf의 역할로 `./configure && make` 의 `/configure` 스크립트가 한다.   
  
그럼 같은 일을 CMake에서 하기 위하여 어떻게 해야 할까?  
그래서 이번 글은 CMake 라이브러리 탐험의 이야기이다. 방법은 여러 가지 있으므로 각각에 대해 살펴 보자.
  
  
## find_package 명령을 사용
CMake를 설치하면 cmake 명령뿐만 아니라 많은 모듈을 설치하고, 잘 알려진 라이브러리 검색 모듈도 포함 되어 있다.  
사용 가능한 모듈 목록은`cmake --help-module-list`로 표시 할 수 있다. 다수의 모듈이 표시되는데, 그 중 FindXXX라는 이름으로 되어 있는 것이 탐색용 모듈이다.  
  
여기에 있는 라이브러리이면 find_package 명령을 사용하여 찾을 수 있다. 우선 이것을 사용해 보자.  
  
예로 GTK2를 사용하는 응용 프로그램을 생각한다.  
다음과 같은 GTK2를 사용 소스 파일(main.c)를 만들었다.   
GTK 자체 실험이 아니기 때문에 단순히 gtk_init()를 호출하고 아무것도 하지 않는 프로그램이지만, 빌드에 포함된 경로 등의 지정이 필요하다.  
```
#include <gtk/gtk.h>

int main( int argc, char ** argv) {
    gtk_init(&argc, &argv);
}
```
  
이것을 빌드하는 CMakeLists.txt는 다음과 같다.  
```
cmake_minimum_required(VERSION 2.6)
project(Hello C)
add_executable(hello main.c)
find_package(GTK2 REQUIRED)
include_directories(${GTK2_INCLUDE_DIRS})
target_link_libraries(hello ${GTK2_LIBRARIES})
```
  
find_package 명령을 실행하면 지정된 라이브러리 탐색 모듈(FindGTK2)를 찾고, 이것을 실행한다. 
  
FindGTK2은 이 시스템에 있는 GTK2를 찾고 이 결과를 변수에 넣는다. 이 경우 포함 경로와 링크해야 라이브러리가 GTK2_INCLUDE_DIRS과 GTK2_LIBRARIES에 set 된다.   
  
그리고는 include_directories과 target_link_libraries를 사용하여 지정하면 된다. 또한,이 프로그램에게 GTK2는 필수 라이브러리이므로 REQUIRED를 붙이고 있다.  
  
### find_package 명령 세트 변수
모듈 XXX 대해 find_package 세트 변수는 대체로 다음과 같다. 의미는 보면 알 것이다.  
  
XXX_FOUND  
XXX_INCLUDE_DIRS  또는  XXX_INCLUDES  
XXX_LIBRARIES  또는 XXX_LIBS  
XXX_DEFINITIONS  
  
XXX 부분은 GTK2_FOUND, CURSES_FOUND 등과 같이 모든 대문자인 경우가 많지만, Boost_FOUND 등과 같이 카멜 케이스로 되어 있는 경우도 있다.   
  
구체적으로 어떤 변수가 설정 되는지는 cmake --help-module FindGTK2 를 보면 알 수 있다.  
  
  
  
## find_package 동작에 대한 상세
find_package의 내부 동작에는 두 가지 있으면 모듈 모드와 구성 모드 라고 한다.  
  
find_package(XXX)를 호출하면 cmake는 먼저 FindXXX.cmake 라는 파일을 찾는다. 먼저 ${CMAKE_MODULE_PATH}에 지정된 디렉토리를 찾고, 없으면 /share/cmake-xy/Modules 아래(예 /usr/share/cmake-3.5/Modules  아래)를 찾는다. 그래도 없으면 XXXConfig.cmake 나 xxx-config.cmake을 찾는다.
  
FindXXX.cmake를 사용하는 것이 모듈 모드로, XXXConfig.cmake/xxx-config.cmake를 사용하는 것이 구성 모드이다. 
  
앞의 예에서 FindGTK2.cmake을 사용했기 때문에 모듈 모드를 사용한 것이다.   
모듈 모드 및 구성 모드의 차이는 단순한 우선 순위의 차이뿐만 아니라, 작성자 및 처리 내용(일반적으로)이 다르다.   
모듈 모드는 파일 이인 FindXXX라는 이름이 나타내는대로 지정된 라이브러리를 찾을 것이며, 즉 이 라이브러리를 만들고/설치한 사람 이외가 쓰는 것이다.  
  
한편 구성 모드의 파일은 검색 대상 라이브러리 자신에 의해 배치 되는 것을 상정하고 있다. 아마 이 라이브러리의 설치 등이 있는 경우일 것이다. 
  
따라서 XXXConfig.cmake/xxx-config.cmake는 일반적으로 라이브러리를 "검색"할 수 없다. 이 라이브러리 자신이 놓인 것이라면 라이브러리가 어디에 설치했는지는 알고 있을 것이므로 단순히 하드 코드된 값이 적혀 있는 것이다.  
  
그러나 현재는 구성 모드 .cmake을 두는 라이브러리는 별로 없는 것 같다. 실제로 Ubuntu-16.04를 봐도 이런 파일은 보이지 않는다.   
그리하여 구성 모드를 사용하는 것은 CMake가 더 확산 되면 자신이 직접 만드는 경우에만 보인다.  
  
  
  
## pkg_check_modules를 사용
find_package을 사용할 수 없는 경우 탐색 작업을 직접 수행하게 된다. 예를 들어, 나의 cmake는 FindGTK(GTK1) 및 FindGTK2(GTK2 용)는 있지만, GTK3는 찾는 모듈이 없다.   
어쩔 수 없기 때문에 GTK3을 스스로 찾을 것이지만, pkg_check_modules를 사용할 수 있다면 그렇게 어렵지 않다.  
그리고 여기 pkg_check_modules 설명하기 전에 그 내부에서 사용하는 pkg-config를 설명한다.  
  
  
### pkg-config는
pkg-config는 포함 경로 및 라이브러리 경로 등 이 라이브러리를 사용하는 응용 프로그램을 빌드하는 데 필요한 정보를 제공 해주는 도구이다.   
freedesktop.org라는 Unix 계열 OS 데스크탑 환경의 공통 사양 도구를 제공하는 단체가 만든 것 같다(그래서 pkg-config 자체는 CMake와 무관).   
  
Linux, BSD, Mac OS X, Windows용 MSYS 등 많은 환경에서 사용할 수 있기 때문에 이것을 사용하면 쉽게 이식성 있는 빌드 환경을 만들 수 있다.  
사용법은 man을 보기를 바라지만, `pkg-config --cflags`로 이 라이브러리의 이용에 필요한 컴파일러 옵션이 표시 된다.  
--libs 옵션으로 링커 옵션이 표시된다.  
<pre>
$ pkg-config --cflags gtk+-3.0
-pthread -I/usr/include/gtk-3.0 -I/usr/include/at-spi2-atk/2.0 -I/usr/include/at-spi-2.0 -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include -I/usr/include/gtk-3.0 -I/usr/include/gio-unix-2.0/ -I/usr/include/mirclient -I/usr/include/mircommon -I/usr/include/mircookie -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng12 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
 
$ pkg-config --libs gtk+-3.0
-lgtk-3 -lgdk-3 -lpangocairo-1.0 -lpango-1.0 -latk-1.0 -lcairo-gobject -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0
</pre>
  
GTK3을 사용하는 애플리케이션 이라면 다음과 같이 하면 빌드있다.  
<pre>
$ gcc `pkg-config --cflags gtk+-3.0` main.c `pkg-config --libs gtk+-3.0`
</pre>
  
또한, 지정 가능한 라이브러리 목록은`pkg-config --list-all`로 표시 할 수 있다.  
  
  
### CMake에서 pkg-config 사용
그리고, 이 편리한 pkg-config를 CMake에서 사용할 수 있도록 한 것이 PkgConfig 모듈이다. 사용법은 샘플을 보면 알 것이다.    
```
cmake_minimum_required(VERSION 2.6)
project(Hello C)
add_executable(hello main.c)
 
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 gtk+-3.0 REQUIRED)
include_directories(${GTK3_INCLUDE_DIRS})
target_link_libraries(hello ${GTK3_LIBRARIES})
```
  
우선 find_package에서 이 모듈을 찾는다. 실제로 pkg-config를 사용하는 것은 pkg_check_modules 명령에서 pkg_check_modules( )과 같이 지정한다.

`pkg-config --list-all`에 나오는 이름에서는 결과를 저장할 변수의 접두사이다. 여기에서는 "GTK3" 라고 했기 때문에 GTK3_INCLUDE_DIRS 등의 변수가 set 된다.  
  
본대로, FindXXX를 사용한 경우와 별로 차이는 없다. 사실 기본적으로 설치되는 FindXXX.cmake도 내부에서는 pkg_check_modules을 사용하는 것도 있다.  
  
  
  
## FindXXX.cmake를 복사하여 사용
find_package를 사용할 수 없는 경우의 또 다른 해결책으로 다른 사람이 만든 명령을 복사하여 사용하는 것이있다. 웹에서 찾아 본다.  
  
있다면 아마도 FindGTK3.cmake 라는 이름이므로 이 이름에서 구글링 해 보면 몇 가지 그럴듯한 것이 나온다. 그 중에서도 Chromium에 포함 되어있 는 것이 있다면 믿을 수 있을 것 같은 느낌이 든다, 이것을 복사하여 FindGTK3.cmake로 저장하자.  
  
우선 프로젝트의 제일 위에 cmake 디렉토리를 만들고 그 안에 놓아둔다.  
<pre>
test/
  +- CMakeLists.txt
  +- main.c
  +- cmake/
       +- FindGTK3.cmake

</pre>
  
CMakeLists.txt는 다음과 같다.  
```
cmake_minimum_required(VERSION 2.6)
project(Hello C)
add_executable(hello main.c)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(GTK3 REQUIRED)
include_directories(${GTK3_INCLUDE_DIRS})
target_link_libraries(hello ${GTK3_LIBRARIES})
```
  
스스로 둔 FindXXX.cmake를 사용하는 경우 CMAKE_MODULE_PATHfmf 지정할 필요가 있다. 
위쪽에도 썼던대로 모듈의 탐험은 CMAKE_MODULE_PATH와 CMake가 설치된 디렉토리 Modules 아래뿐이다.  
만약 CMakeLists.txt과 같은 디렉토리에 배치해도 자동으로 읽어주지 않는다.  
  
  
  
## REQUIRED과 QUIET
find_package, pkg_check_modules 일반적인 옵션에 REQUIRED과 QUIET 가 있다.  
find_package와 pkg_check_modules에 REQUIRED를 지정하지 않으면 찾을 수 없는 경우 메시지는 내지만 처리는 계속한다. 
REQUIRED를 지정하면 발견되지 않았던 경우는 처리가 거기에서 멈추고 cmake 명령 자체의 반환 값도 1이다. 또한 QUIET를 지정하면 없으면 메시지 출력이 억제된다.  
또한 find_package에 REQUIRED를 지정하지 않은 경우에도 FindXXX.cmake 자체가 없는 경우는 에러가 되고, 거기서 작업이 중단된다.  
  
다음에 예를 든다. 현재 내 환경에 Qt4가 들어 있지 않다.  
```
find_package(PkgConfig REQUIRED)
pkg_check_modules(XXX xxx)          # xxx라는 모듈이 없을 때를 위한 에러 메시지가 나오지만 처리는 계속 실행
pkg_check_modules(XXX xxx QUIET)    # 에러 메시지를 출력하지 않고 계속 실행
pkg_check_modules(XXX xxx REQUIRED) # 에러가 되면 처리가 중단된다
find_package(Qt4)                   # 에러 메시지를 추렭하고 처리 계속 실행
find_package(Qt4 QUIET)             # 에러 메시지를 내지 않고 계속 실행
find_package(Qt4 REQUIRED)          # 에러가 되면 처리가 중단된다
find_package(YYY)                   # FindYYY.cmake가 없기 때문에 REQUIRED가 붙지 않아도 에러로 종료
```
  
  
  
## 참고 자료
- [CMake : How To Find Libraries](https://cmake.org/Wiki/CMake:How_To_Find_Libraries )
- [find_package의 공식 설명서](https://cmake.org/cmake/help/v3.0/command/find_package.html )
- [pkg_check_modules의 공식 설명서](https://cmake.org/cmake/help/v3.0/module/FindPkgConfig.html )
  
