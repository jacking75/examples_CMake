# 빌드 시에 공개 헤더 파일을 한 장소에 복사 하고 싶다
  
아래와 같은 프로젝트가 있고  

.
├── lib
│     ├── libhoge
│     │     ├── include
│     │     │     ├── hoge.h
│     │     │     └── hoge_common.h
│     │     ├── src
│     │     │     ├── src1.c
│     │     │     ├── src2.c
│     │     │     └── internal.h
│     │     └── CMakeLists.txt
│     └── libfuga
│            ├── fuga.c
│            ├── fuga.h
│            └── CMakeLists.txt
├── src
│     └── main.c
└── CMakeLists.txt

  
빌드를 실행하기 전에 export_headers 라는 디렉토리를 만들고  
- libhoge/hoge.h
- libhoge/hoge_common.h
- libfuga/fuga.h
을 복사해서 저장하고 싶다  라는 경우  
  
add_custom_target()를 사용하여 명령어를 정의하고, 게다가 add_dependencies() 로 의존 관계를 만들어 두면 빌드 전에 무엇인가를 실행 시킬 수 있다.  
즉 예를들면 lib/libhoge/CMakeLists.txt 중에
  
```
add_library (libhoge SHARED src/src1.c src/src2.c)
add_custom_target (copy_header_hoge_h
                   COMMAND
                   ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/include/hoge.h
                                            ${CMAKE_BINARY_DIR}/export_headers/libhoge/hoge.h)
add_dependencies(libhoge copy_header_hoge_h)
 
라고 써 두면 libhoge를 빌드 하기 전에 include/hoge.h를 복하할 수 있다. 
매크로를 만들어 보자 
   
lib/libhoge/CMakeLists.txt 
 
set (TARGET_NAME libhoge)
set (SRCS
    src/src1.c
    src/src2.c
)
set (C_INCLUDES
    include
    src
)
  
# 복사할 헤더 파일 리스트
set (EXPORT_HEADER_FILES
    include/hoge.h
    include/hoge_common.h
)
# export_headers内のサブディレクトリ名
set (EXPORT_HEADER_DIR libhoge)
  
include_directories (${C_INCLUDES})
add_library (${TARGET_NAME} SHARED ${SRCS})
set_target_properties (${TARGET_NAME}
                       PROPERTIES PREFIX
                       "")
# 매크로이므로 인수에 리스트를 넘기 때는 ""으로 감싸는 것에 주의.
copy_headers (${TARGET_NAME} ${EXPORT_HEADER_DIR} "${EXPORT_HEADER_FILES}")
   
     
lib/libfuga/CMakeLists.txt 
 
set (TARGET_NAME libfuga)
set (SRCS
    fuga.c
)
  
# 복사할 헤더 파일 리스트
set (EXPORT_HEADER_FILES
    fuga.h
)
# export_headers 내의 서브 디렉토리 이름
set (EXPORT_HEADER_DIR libfuga)
  
add_library (${TARGET_NAME} SHARED ${SRCS})
set_target_properties (${TARGET_NAME}
                       PROPERTIES PREFIX
                       "")
copy_headers (${TARGET_NAME} ${EXPORT_HEADER_DIR} "${EXPORT_HEADER_FILES}")
   
   
./CMakeLists.txt
 
# ---------- 공통 설정
  
# 버전은 적당하게
cmake_minimum_required (VERSION 2.6.0)
  
project (piyo-project)
  
set (CMAKE_BUILD_TYPE Debug)
set (CMAKE_C_FLAGS "-Wall -Werror")
  
set (GLOBAL_HEADER_DIR ${CMAKE_BINARY_DIR}/export_headers)
  
  
# ---------- 매크로 정의
  
# 헤더를 복사하는 매크로
macro (copy_headers MODULE_NAME OUT_DIR_PREFIX FILE_LIST)
  
    foreach (HEADER_FILE ${FILE_LIST})
        # 파일 명을 취득
        get_filename_component (FNAME ${HEADER_FILE} NAME)
        # 파일 명의 일부를 사용하여 타겟 명을 생성
        get_filename_component (TEMP_TARGET ${HEADER_FILE} NAME_WE)
        set (TEMP_TARGET copy_header_${OUT_DIR_PREFIX}_${TEMP_TARGET})
        # 복사하는 명령어
        add_custom_target (
            ${TEMP_TARGET}
            COMMAND
            ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER_FILE}
                                     ${GLOBAL_HEADER_DIR}/${OUT_DIR_PREFIX}/${FNAME}
        )
        # 의존 관계를 지정(라이브러리 빌드 전에 복사가 실행되도록 한다)
        add_dependencies (${MODULE_NAME} ${TEMP_TARGET})
    endforeach (HEADER_FILE ${FILE_LIST})
  
endmacro (copy_headers MODULE_NAME OUT_DIR_PREFIX FILE_LIST)
  
  
# ---------- 서브 디렉토리 등록
  
set (SUB_DIRS
    lib/libhoge
    lib/libfuga
)
foreach (DIR ${SUB_DIRS})
    add_subdirectory (${DIR})
endforeach (DIR ${SUB_DIRS})
  
  
# ---------- 메인 실행 파일
  
set (TARGET_EXE_NAME piyoproj)
set (SRCS
    src/main.c
)
set (SHARED_LIBS
    libhoge
    libfuga
)
  
include_directories (${GLOBAL_HEADER_DIR})
add_executable (${TARGET_EXE_NAME} ${SRCS})
target_link_libraries (${TARGET_EXE_NAME} ${SHARED_LIBS})
```  