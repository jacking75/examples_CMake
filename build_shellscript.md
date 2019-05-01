# cmake에서 빌드 시에 쉘스크립트를 실행하기
출처: https://qiita.com/termoshtt/items/7f7090f9d0c52bcd2630
  
  
## 다른 파일을 생성하지 않는 경우
ctags 및 doxygen처럼, 다른 소스의 컴파일에 관계 없는 것을 생성하기 위해서는 'add_custom_target'을 사용한다 :  
```
add_custom_target(ctags ALL COMMAND "ctags" "-R" "-f" ".tags" "--languages=C++,python" "--exclude='CMake*'")
add_custom_target(document COMMAND "doxygen" "doc/Doxyfile")
```
  
COMMAND 아래에 실행하고 싶은 쉘스크립트를 기술한다.  
ALL을 붙이면 매회 make에서 자동적으로 실행된다.  
  
  
  
## 파일을 생성하는 경우
예를 들면 프로젝트 KSE에서 사용하는 프로토콜 버퍼 코드를 생성하고 싶은 경우 'KSE.pb.cc' 와 'KSE.pb.h'가 생성되고, 이것을 컴파일할 필요가 있다.  
이 경우는 'add_custom_command'를 사용한다:  
```
add_custom_command(
  OUTPUT KSE.pb.cc KSE.pb.h
  DEPENDS KSE.proto
  COMMAND "protoc" "KSE.proto" "--cpp_out=." "--python_out=."
)
```
  
이것에 의해 타겟에 'KSE.pb.cc', 'KSE.pb.h'가 추가 되었다.  
```
make KSE.pb.cc
```
  
로 'KSE.pb.cc'가 생성된다.  
물론 의존 관계는 자동으로 해결 되므로  
```
add_library(KSE STATIC
  logger.cpp
  KSE.pb.cc
)
```
  
처럼 어떤 경우는 멋대로 'KSE.pb.cc'를 생성해준다.  