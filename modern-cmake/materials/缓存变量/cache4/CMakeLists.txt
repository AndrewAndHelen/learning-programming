cmake_minimum_required(VERSION 3.18)
 
# 设置工程名称
set(PROJECT_NAME KAIZEN)
 
# 设置工程版本号
set(PROJECT_VERSION "1.0.0.10" CACHE STRING "默认版本号")
 
# 工程定义
project(${PROJECT_NAME}
   LANGUAGES CXX C
   VERSION ${PROJECT_VERSION}
)
 
# 打印开始日志
message(STATUS "\n########## BEGIN_TEST_CACHE_VARIABLE")
 
message("${PROJECT_NAME}_GLOBAL_VAR_1: ${${PROJECT_NAME}_GLOBAL_VAR}")
 
# 修改缓存变量
set(${PROJECT_NAME}_GLOBAL_VAR "I am a cache variable" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("${PROJECT_NAME}_GLOBAL_VAR_2: ${${PROJECT_NAME}_GLOBAL_VAR}")
 
# 打印结束日志
message(STATUS "########## END_TEST_CACHE_VARIABLE\n")