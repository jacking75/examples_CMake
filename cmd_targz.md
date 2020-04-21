# CMake로 소스 파일 전달을 위해 압축 파일로 만들기
[출처](https://qiita.com/yuuka-kanakubo/items/92773de8c5e21f4b704b ) 
   
프로젝트를 빌드하면서 이 프로젝트에 사용된 소스 코드들을 전달 등의 목적으로 압축 파일을 만드는 명령어로 CMake로 한다.  
    
```
add_custom_target(
    create_targz ALL 
    COMMAND mkdir ${TARGETV}
    COMMAND cp CMakeLists.txt main.cpp coffee.cpp tea.cpp tea.h coffee.h def.h break_fast.h  ${TARGETV}/
    COMMAND ${CMAKE_COMMAND} -E tar czf ${TARGETV}.tar.gz ${TARGETV}
    COMMAND rm -rf ${TARGETV}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    DEPENDS morning
)
```
  
`COMMAND`를 사용하여 외부 명령어를 사용한다.  
[여기](https://github.com/yuuka-kanakubo/morning_cmakelists )에 샘플 코드가 있다.  
  
  
