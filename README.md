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



### 정리해야 하는 것들
- CMakeの勧め
http://www.wakayama-u.ac.jp/~chen/cmake/cmake.html
- CMakeLists Tutorial
https://gist.github.com/UnaNancyOwen/47159d73b480f16b846a
- Visual Studio 2017にOpenCV3.2.0とopencv_contribを導入する方法
http://qiita.com/tomochiii/items/fa26404ebc5fcd4481b9
- CMake 簡易まとめ http://qiita.com/janus_wel/items/a673793d448c72cbc95e
- cmakeでビルド時にシェルスクリプトを実行する http://qiita.com/termoshtt/items/7f7090f9d0c52bcd2630
- C++11 向け gcc オプション http://qiita.com/janus_wel/items/07140fcd1127b568087d
- CMake/C++ プロジェクトについて GCC または Clang のバージョンに併せた最新の -std オプションを与える CMake の記述例  http://qiita.com/usagi/items/a12d5bf08f4a7a13329d
- CMake で Boost を静的リンクする http://qiita.com/janus_wel/items/cbad087b5c3a13a57625
- cmakeでboostライブラリのリンク順を指定 http://qiita.com/YukiMiyatake/items/345c665b11a7687a7968
- CMakeのin-sourceとouf-of-sourceの違い http://qiita.com/tttamaki/items/2830e1df4efa48f247ac
- non-trivialなBoost.Logの使い方(4. cmakeによるコンパイル編) http://qiita.com/termoshtt/items/9c856562fe8a8c42560c
- How to compile a 32-bit binary on a 64-bit linux machine with gcc/cmake http://stackoverflow.com/questions/1272357/how-to-compile-a-32-bit-binary-on-a-64-bit-linux-machine-with-gcc-cmake
- CMakeでプロジェクトを作成する(Windows) http://qiita.com/Lacty/items/d02eeba72dde2b875c31  


### 참고
- [qitta](http://qiita.com/tags/cmake)
- [awesome-cmake](https://github.com/onqtam/awesome-cmake)
- cmake の使い方 http://www.cs.gunma-u.ac.jp/~nagai/wiki/index.php?cmake%20%A4%CE%BB%C8%A4%A4%CA%FD
- CMake: プリコンパイル済みヘッダーの作成と利用 http://qiita.com/mrk_21/items/264f6135679239ff018a
- cmake에서 정적 라이브러리를 하나로 합치기 http://ospace.tistory.com/539
- Clang http://www.cmake.org/Wiki/CMake_Cross_Compiling
- CMakeLists.txt から始めよう - Visual Studio http://qiita.com/nonbiri15/items/c77c046f0e10f8140cfe
- CMake Advent Calendar 2014 http://qiita.com/advent-calendar/2014/cmake
