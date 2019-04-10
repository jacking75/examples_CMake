# 설정 파일 만들기
출처: https://www.wagavulin.jp/entry/2011/11/27/222654
  
  
## 조건 있는 컴파일 지정 방법
#include <cstdio>
 
int main(){
#if USE_FEATURE_X
    printf("use feature-x\n");
#else
    printf("do not use feature-x\n");
#endif
    return 0;
}


이 코드는 USE_FEATURE_X 값에 의해 컴파일 결과가 달라질 것이지만, 이 값을 결정하는 방법은 2가지가 있다.
- 컴파일러 옵션으로 지정 (-DUSE_FEATURE_X = 1)
- 소스 코드의 다른 위치에서 정의 (#define USE_FEATURE_X 1을 추가)


컴파일 결과는 같게 되므로 일일이 파일을 편집 할 필요가 없는 컴파일러 옵션을 지정하는 것이 편하다.

그러나 경우에 따라서는 그것이 바람직하지 않은 경우도 있다. 예를 들어, 실행 파일이 아닌 라이브러리(libHoge.so)를 만들고 있다고 하자.



다음과 같은 구성이다.

소스 파일 
	Hoge.cpp 
헤더 파일 
	Hoge.h 이것은 라이브러리의 사용자에 대한 공개 헤더이기도 하다  
그리고 Hoge.h에 다음과 같은 조건 분기가있다.
```
class Hoge {
public:
    Hoge();
    ~Hoge();
private:
    int n;
#if USE_FEATURE_X
    int x;
#endif
};
```

이런 경우 당연히 libHoge.so 빌드 시와 이것을 사용하는 응용 프로그램 빌드 시 USE_FEATURE_X 값을 모아 두지 않으면 안된다.

그러나 libHoge.so 빌드 할 때 컴파일러 옵션 -DUSE_FEATURE_X = 1로 한 경우 응용 프로그램을 빌드 할 때 같은 옵션을 켜야 한다.

이렇게 되면 응용 프로그램 개발자는 자신이 사용하는 모든 라이브러리에 매크로 설정을 파악해 두어야 하고, 당연히 파악하지 못하여 파산하게 될 것이다.

이런 경우 USE_FEATURE_X를 헤더 파일에 정의 해 두지 않으면 안된다. 따라서 환경 의존 부분이 헤더 파일은 CMake에 자동 생성 되도록 한다.

헤더 파일의 바탕이 되는 뼈대를 준비해 두고 libHoge.so 빌드 시 지정한 옵션에 따라 실제로 사용되는 헤더 파일을 생성하는 것이다.
물론 HogeConfig.h를 자동 생성 시켜도 좋지만, 이런 것은 전용의 별도 파일에 두면 보기가 좋아진다. 따라서 환경에 맞는 설정을 기술하는 HogeConfig.h을 마련하기로 하자.


이것을 Hoge.h에서 포함하면 된다.



## CMake에서 설정 파일을 만드는 방법
우선 HogeConfig.h의 양식이 되는 HogeConfig.h.in을 준비한다.

#ifndef HOGECONFIG_H
#define HOGECONFIG_H
 
#define USE_FEATURE_X @USE_FEATURE_X@
 
#endif // HOGECONFIG_H


@로 둘러 싸인 부분은 CMake로 대체되는 부분이다. 또한 CMakeLists.txt은 다음과 같이한다.

cmake_minimum_required(VERSION 2.8)
configure_file("HogeConfig.h.in" "HogeConfig.h")
add_executable(myapp main.cpp Hoge.cpp)


configure_file 명령으로 프로토타입이 되는 파일과 자동 생성하는 파일의 이름을 지정한다.

파일 이름은 무엇이든 좋지만, GNU Autotools의 관습에 따라 {원래 파일 이름}.in 하는 것이 좋겠다.

이제 HogeConfig.h는 USE_FEATURE_X 값이 반영된 것이 된다.







## 경로 설정

여기에서는 configure_file 명령의 인수로 단순히 파일 이름만 설명했지만, 실제로는 PROJECT_SOURCE_DIR 이하인 것을 설명하는 것이 좋다.
```
configure_file(
  "${PROJECT_SOURCE_DIR}/HogeConfig.h.in"
  "${PROJECT_SOURCE_DIR}/HogeConfig.h"
)
```
