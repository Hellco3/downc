cmake_minimum_required(VERSION 3.10)
# set the project name and version
project(CLINET_TOOL)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(src TOOLSRC)
aux_source_directory(3rdparty/mxml/src MXMLSRC)
aux_source_directory(src/filecompare FSRC_LIST)

set(
    SRC_LIST
    "${TOOLSRC}"
    "${MXMLSRC}"
)

set(INC_DIR
    "include"
    "3rdparty/mxml/include"
    )

set(LIB_DIR
    /lib64
    "lib"
    )
add_executable(downc ${SRC_LIST})
add_executable(filecompare ${FSRC_LIST})

target_link_directories(downc PUBLIC ${LIB_DIR})

target_link_libraries(downc PUBLIC
                      pthread
                      log4cpp
                      curl
                      )
target_include_directories(downc PUBLIC ${INC_DIR})
