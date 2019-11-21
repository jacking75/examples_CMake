# 빌드 옵션
[출처](https://www.wagavulin.jp/entry/2011/11/27/222650 )
  
## option 명령
option 명령을 사용하여 온·오프 지정하는 옵션이 가능하게 된다.  
예를 들어, 추가 기능 feature-x가 있고, 컴파일 시 옵션으로 전환 할 수 있도록 하고 싶다고 하자. 소스 파일은 #if 의한 전환이 들어가 있다.  
```
#include <cstdio>
 
int main () {
 #if ENABLE_FEATURE_X
    printf ( "feature-x enabled \ n " );
 #else
    printf ( "feature-x disabled \ n " );
 #endif
    return  0 ;
}
```
  
이런 경우 option 명령을 사용할 수있다.  
```
cmake_minimum_required(VERSION 2.8)
add_executable(myapp main.cpp)
option(USE_FEATURE_X "Use feature x" ON)
if(USE_FEATURE_X)
  add_definitions(-DENABLE_FEATURE_X)
endif()
```
  
option 명령의 인수는 옵션 변수 이름, 설명, 초기 값이다.  
그리고 옵션 변수 USE_FEATURE_X가 ON 때 add_definitions에서 ENABLE_FEATURE_X 매크로가 정의 되도록 하고 있다.  
USE_FEATURE_X의 ON / OFF는 다음 중 하나를 지정한다.  
- cmake 명령 줄 인수로 -DUSE_FEATURE_X=ON/OFF를 붙인다
- CMakeLists.txt 중에서 set 명령을 사용
- 명시적으로 지정하지 않는다 → 초기 값 또는 캐시 된 값을 사용
  
  
실행하면 다음과 같이 된다(중간의 메시지는 생략하고 myapp 결과만 올린다).
cmake를 처음 실행에서 아무것도 지정하지 않는다 -> 초기 값 (ON)이 된다.  
<pre>
> cmake . && make && ./myapp
(snip)
feature-x enabled
</pre>
  
명령 줄 옵션을 OFF로 한다.  
  
<pre>
> cmake -DUSE_FEATURE_X=OFF . && make && ./myapp
(snip)
feature-x disabled
</pre>
  
CMakeLists.txt 중에서 지정한다. 쓰기는 option 명령과 if 명령 사이 이다.  
```
cmake_minimum_required(VERSION 2.8)
add_executable(myapp main.cpp)
option(USE_FEATURE_X "Use feature x" ON)
set(USE_FEATURE_X OFF)
if(USE_FEATURE_X)
  add_definitions(-DENABLE_FEATURE_X)
endif()
```
  
CMakeLists.txt를 바꾸는 것으로 cmake는 자동으로 실행 되므로 단순히 make 하면 된다.  
<pre>
> make && ./myapp
(snip)
feature-x disabled
</pre>
  
   
## option 명령의 소스 파일 추가
위의 CMakeLists.txt 에서 add_definitions 의한 매크로 정의의 추가를 실시했지만, 물론 include_directories와 target_link_libraries을 사용하여 인클루드 패스와 라이브러리를 추가 할 수도있다. 그러나 add_executable과 add_library는 1 타겟 당 1회 밖에 사용할 수 없기 때문에 그대로는 추가 할 수 없다.  
소스 파일의 목록을 적절한 변수에 넣고, 마지막에 정리해서 add_executable에 넣으면 좋을 것이다.  
예를 들어 "기본적으로 main.cpp와 sub1.cpp가 빌드 대상으로, USE_FEATURE_Y이 ON 일때는 sub2.cpp과 sub3.cpp도 포함" 이라는 경우는 다음과 같이 하면 좋을 것이다.  
```
cmake_minimum_required(VERSION 2.8)
set(myapp_sources main.cpp sub1.cpp)
option(USE_FEATURE_Y "Use feature y" ON)
if(USE_FEATURE_Y)
  set(myapp_sources ${myapp_sources} sub2.cpp sub3.cpp)
endif()
add_executable(myapp ${myapp_sources})
```
  
  
## 3개 이상 선택 할 수 있는 옵션
option 명령을 사용하면 선택은 ON/OFF 두 가지 선택된다.  
그러나 옵션에 따라 3개 이상 경우도 있을 것이다.  
예를 들어 어떤 통신 하는 응용 프로그램에서 SSL 암호화 기능이 있다고한다. 이 때 빌드 옵션으로 OpenSSL을 사용, GnuTLS를 사용, 암호화 기능은 사용하지 않는다,의 세 가지 옵션을 마련하고 싶은 경우이다.  
소스 파일은 아래와 같다.  
```
#include <cstdio>
 
int main() {
#if USE_OPENSSL
    printf("use OpenSSL\n");
#elif USE_GNUTLS
    printf("use GnuTLS\n");
#else
    printf("not encrypted\n");
#endif
    return 0;
}
```
  

CMakeLists.txt는 다음과 같이 하면 좋을 것 같다.  
```
cmake_minimum_required(VERSION 2.8)
 
# ssllib가 set 되어 있지 않다면 openssl 로 한다
if(NOT DEFINED ssllib)
  set(ssllib openssl)
endif()
 
if(ssllib STREQUAL openssl)
  add_definitions(-DUSE_OPENSSL)
elseif(ssllib STREQUAL gnutls)
  add_definitions(-DUSE_GNUTLS)
elseif(ssllib STREQUAL none)
elseif(DEFINED ssllib)
  # openssl, gnutls, none 이 아닌 것이 set 되어 있다면 에러
  message(FATAL_ERROR "invalid ssllib")
endif()
 
add_executable(myapp main.cpp)
```
  

이것으로 아래처럼 사용할 수 있다.  
<pre>
> cmake . && make && ./myapp                   # 첫 실행으로 아무것도 지정하지 않는다
(snip)                                         #
use OpenSSL                                    #   -> OpenSSL가 사용된다
> cmake -Dssllib=gnutls . && make && ./myapp   # gnutls를 지정한다
(snip)                                         #
use GnuTLS                                     #   -> GnuTLS가 사용된다
> cmake . && make && ./myapp                   # 다시 아무것도 지정하지 않는다
(snip)                                         #
use GnuTLS                                     #   -> 이 때는 전회의 것이 된다
> cmake -Dssllib=none . && make && ./myapp     # none을 지정한다
(snip)                                         #
not encrypted                                  #   -> 암호화는 사용되지 않는다
> cmake -Dssllib=hoge . && make && ./myapp     # 틀린 값을 지정한다
CMake Error at CMakeLists.txt:15 (message):    #   -> 에러가 된다
  invalid ssllib                               #
                                               #
                                               #
-- Configuring incomplete, errors occurred!    #
</pre>
  
