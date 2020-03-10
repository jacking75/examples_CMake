[출처](https://qiita.com/janus_wel/items/cbad087b5c3a13a57625 )  
  
## CMake에서 Boost를 정적 링크 하기
CMake에서는 정적 링크하기 위해서 대상 파일 각각에 add_library 와 set_target_properties 라는 준비물이 필요하다.  
Boost처럼 많은 것을 하나하나 대처하기 힘들어서 스크립트를 써 보았다.  
  
generate-boost-static-settings.rb
```
#!/usr/bin/env ruby

#
# generate-boost-static-settings.rb
#
#   ./generate-boost-static-settings.rb > boost-static.cmake
#

CMAKE_VERSION = 2.8
LIB_PATH = '/usr/local/lib'
CMAKE_TEMPLATE = <<EOS
# ${libname}
add_library(boost_${libname}_static
    STATIC
    IMPORTED
)

set_target_properties(boost_${libname}_static
    PROPERTIES
    IMPORTED_LOCATION #{LIB_PATH}/libboost_${libname}.a
)

EOS

puts "cmake_minimum_required(VERSION #{CMAKE_VERSION})"
puts

Dir.glob(LIB_PATH + '/*') {|file|
    if matched = /libboost_(\w+)\.a/.match(file)
        libname = matched[1]
        puts CMAKE_TEMPLATE.gsub(/\$\{libname\}/, libname)
    end
}
```
  
`LIB_PATH`는 적절하게 바꾸어야 한다.  
이것을 실행하면 다음과 같은 출력이 되므로 파일로 보족해서 프로젝트 루트의 CMakeLists.txt 에서 include 하던가, 직접 CMakeLists.txt에 쓴다.  
  
```
cmake_minimum_required(VERSION 2.8)

# atomic
add_library(boost_atomic_static
    STATIC
    IMPORTED
    )

set_target_properties(boost_atomic_static
    PROPERTIES
    IMPORTED_LOCATION /usr/local/lib/libboost_atomic.a
    )

#
# snip
#

# unit_test_framework
add_library(boost_unit_test_framework_static
    STATIC
    IMPORTED
    )

set_target_properties(boost_unit_test_framework_static
    PROPERTIES
    IMPORTED_LOCATION /usr/local/lib/libboost_unit_test_framework.a
    )
```
  
링크를 지정할 때는 보통 target_link_libraries 에 boost_<target>_static 을 지정한다.  
Boost Filesystem을 정적 링크하고 싶은 경우는 아래처럼 한다.  
```
# filesystem
add_library(boost_filesystem_static
    STATIC
    IMPORTED
    )

set_target_properties(boost_filesystem_static
    PROPERTIES
    IMPORTED_LOCATION /usr/local/lib/libboost_filesystem.a
    )

add_executable(my-command
    main.cpp
    )

target_link_libraries(my-command
    boost_filesystem_static
    )
```
  
  
## 참고할 글
- [CMake can use a local Boost ](https://scaron.info/blog/cmake-can-use-a-local-boost.html )
- [How do you add Boost libraries in CMakeLists.txt?](https://stackoverflow.com/questions/6646405/how-do-you-add-boost-libraries-in-cmakelists-txt  )

  