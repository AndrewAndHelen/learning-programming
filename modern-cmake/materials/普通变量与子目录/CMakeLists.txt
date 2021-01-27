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
message(STATUS "\n########## BEGIN_TEST")
 
# 零个值
set(MY_LOCAL_ZERO_VAR)
message(STATUS "MY_LOCAL_ZERO_VAR: ${MY_LOCAL_ZERO_VAR}")
 
# 1个值
set(MY_LOCAL_VAR "I am a local variable")
message(STATUS "MY_LOCAL_VAR_1: ${MY_LOCAL_VAR}")
 
# 在相同作用域修改普通变量
set(MY_LOCAL_VAR "I am a local variable by modify same scope")
message(STATUS "MY_LOCAL_VAR_2: ${MY_LOCAL_VAR}")
 
# 多个值
set(MY_LOCAL_LIST_VAR "kaizen" "baidu" "C++" "developer")
message(STATUS "MY_LOCAL_LIST_VAR: ${MY_LOCAL_LIST_VAR}")
 
# 调用子目录src
message(STATUS "\n########## Begin add_subdirectory src")
add_subdirectory(src)
message(STATUS "########## End add_subdirectory src")
# 调用子目录src后打印
message(STATUS "MY_LOCAL_VAR_7: ${MY_LOCAL_VAR}")
# 调用子目录src后打印
message(STATUS "MY_SUB_DIR_VAR_3: ${MY_SUB_DIR_VAR}")
 
# 调用子目录src_parent_scope
message(STATUS "\n########## Begin add_subdirectory src_parent_scope")
add_subdirectory(src_parent_scope)
message(STATUS "########## End add_subdirectory src_parent_scope")
# 调用子目录src_parent_scope后打印
message(STATUS "MY_LOCAL_VAR_8: ${MY_LOCAL_VAR}")
# 调用子目录src_parent_scope后打印
message(STATUS "MY_SUB_DIR_VAR_4: ${MY_SUB_DIR_VAR}")
 
# 打印结束日志
message(STATUS "\n########## END_TEST\n")
 