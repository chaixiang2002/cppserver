cmake_minimum_required(VERSION 3.10)
project(netserver)

set(CMAKE_CXX_STANDARD 20)

# 加入 toml++的库
# include(FetchContent)
# FetchContent_Declare(
#     tomlplusplus
#     GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
#     GIT_TAG v3.4.0
# )
# FetchContent_MakeAvailable(tomlplusplus)

include_directories(include) # 添加头文件目录

file(GLOB LIB_SOURCES "lib/*.cpp")  # 定义 文件变量 LIB_SOURCES
add_library(net_cx ${LIB_SOURCES})  # 变成一个库

file(GLOB TEST_SOURCES "Test/*.cpp") # 定义 文件变量 TEST_SOURCES
foreach(test_source ${TEST_SOURCES}) #循环
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source}) # 制作成 可执行文件
    target_link_libraries(${test_name} PRIVATE  net_cx )
endforeach()