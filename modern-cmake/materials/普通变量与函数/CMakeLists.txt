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
message(STATUS "\n##########BEGIN_TEST")
 
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
 
# 定义函数func
function(func)
    # 修改父作用域普通变量值
    # 修改前打印
    message(STATUS "MY_LOCAL_VAR_3: ${MY_LOCAL_VAR}")
    # 修改动作
    set(MY_LOCAL_VAR "I am a local variable by modify func")
    # 修改后打印
    message(STATUS "MY_LOCAL_VAR_4: ${MY_LOCAL_VAR}")
    # 函数作用域内定义普通变量不加选项PARENT_SCOPE
    set(MY_FUNC_VAR "I am a func inner variable")
    # 函数作用域打印定义变量值
    message(STATUS "MY_FUNC_VAR_1: ${MY_FUNC_VAR}")
endfunction()
 
# 定义函数func_parent_scope
function(func_parent_scope)
    # 修改父作用域普通变量值
    # 修改前打印
    message(STATUS "MY_LOCAL_VAR_5: ${MY_LOCAL_VAR}")
    # 修改动作
    set(MY_LOCAL_VAR "I am a local variable by modify func_parent_scope" PARENT_SCOPE)
    # 修改后打印
    message(STATUS "MY_LOCAL_VAR_6: ${MY_LOCAL_VAR}")
    # 函数作用域内定义普通变量加选项PARENT_SCOPE
    set(MY_FUNC_VAR "I am a func inner variable" PARENT_SCOPE)
    # 函数作用域打印定义变量值
    message(STATUS "MY_FUNC_VAR_2: ${MY_FUNC_VAR}")
endfunction()
 
# 调用函数func
message(STATUS "\n########## Begin call func")
func()
message(STATUS "########## End call func")
# 调用函数func后打印
message(STATUS "MY_LOCAL_VAR_7: ${MY_LOCAL_VAR}")
# 调用函数func后打印
message(STATUS "MY_FUNC_VAR_3: ${MY_FUNC_VAR}")
 
# 调用函数func_parent_scope
message(STATUS "\n########## Begin call func_parent_scope")
func_parent_scope()
message(STATUS "########## End call func_parent_scope")
# 调用函数func_parent_scope后打印
message(STATUS "MY_LOCAL_VAR_8: ${MY_LOCAL_VAR}")
# 调用函数func_parent_scope后打印
message(STATUS "MY_FUNC_VAR_4: ${MY_FUNC_VAR}")
 
# 打印结束日志
message(STATUS "##########END_TEST\n")