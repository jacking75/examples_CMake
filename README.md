# CMake 사용법 정리

### [예제 모음](examples.md)

### 변수 다루기
cmake 에서는 변수를
<pre>
set(HOMHOM "hom")
</pre>
처럼 지정할 수 있다.

이렇게 지정한 변수는 그보다 아래의 아래에는 전해지지만 위로는 전해지지 않는다.
위로 전하고 싶은 경우는 CACHE 지정자를 사용한다.
<pre>
set(HOMHOM "hom" CACHE STRING "homhom is hommm")
</pre>

이 기능을 이용하여 가장 부모 디렉토리에서
<pre>
set(CMAKE_BUILD_TYPE Release)
</pre>
로 해두고, 테스트 용 디렉토리에서는
<pre>
set(CMAKE_BUILD_TYPE Debug)
</pre>
로 하는 것이 가능하다.

PT 문서
https://github.com/jacking75/BackUp/tree/master/PT_Document/CMake

PT 문서 만들 때 아래 글 참고해서 순서대로 만들면 좋을 듯  
CMakeの使い方（その１）
https://qiita.com/shohirose/items/45fb49c6b429e8b204ac 
CMakeの使い方（その２）
https://qiita.com/shohirose/items/637f4b712893764a7ec1 
