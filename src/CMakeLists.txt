cmake_minimum_required(VERSION 3.10)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PineCX
        VERSION 0.1
        DESCRIPTION "pine"
        LANGUAGE C CXX
        )



#人们总是在错误的文件夹中运行CMake，完全破坏了他们的项目或产生奇怪的bug。
#检查你是否在一个已经有CMakeLists.txt的文件夹中运行CMake。
# 重要的是，这捕获了从根目录运行它的常见情况。
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT)
# 将${PROJECT_BINARY_DIR}/CMakeLists.txt的路径转换为CMake可识别的路径格式，并将结果保存在变量PATH_TO_CMAKELISTS_TXT中。

if(EXISTS "${PATH_TO_CMAKELISTS_TXT}")
    message(FATAL_ERROR "从构建子目录运行CMake !\"mkdir build;光盘构建;cmake . .\“\
    在这个文件夹中创建了一些垃圾文件(CMakeCache.txt, CMakeFiles);你应该删掉这些。")
endif()

# 期望的目录结构。
set(PINE_BUILD_SUPPORT_DIR "${CMKAE_SOURCE_DIR}/build_support")
set(PINE_CLANG_SEARCH_PATH "/usr/local/bin" "/usr/bin" "/usr/local/opt/llvm/bin" "/usr/local/opt/llvm@8/bin" "/usr/local/Cellar/llvm/8.0.1/bin")


# Compiler flags.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -Wextra -std=c++17 -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter -Wno-attributes") #TODO: remove
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -ggdb -fno-omit-frame-pointer -fno-optimize-sibling-calls")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -fPIC")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} -fPIC")

set(GCC_COVERAGE_LINK_FLAGS "-fPIC")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}")

# Output directory.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Includes.
set(PINE_SRC_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/include)
set(PINE_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/test/include)
include_directories(${PINE_SRC_INCLUDE_DIR} ${PINE_TEST_INCLUDE_DIR})
add_subdirectory(src)
add_subdirectory(test)