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
 
# 打印开始日志（include和宏）
message(STATUS "\n########## BEGIN_TEST_INCLUDE_MACRO")
 
# 普通变量1个值
set(MY_LOCAL_VAR "aaa")
message(STATUS "MY_LOCAL_VAR_1: ${MY_LOCAL_VAR}")
 
# 在相同作用域修改普通变量值
set(MY_LOCAL_VAR "bbb")
message(STATUS "MY_LOCAL_VAR_2: ${MY_LOCAL_VAR}")
 
# include 根目录中的camke test_variable_macro.camke文件
message(STATUS "\n########## Begin include camke test_variable_macro")
 
# 默认CMAKE_MODULE_PATH这个值为空，使用时需要追增
message(STATUS "CMAKE_MODULE_PATH_1: ${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})
message(STATUS "CMAKE_MODULE_PATH_2: ${CMAKE_MODULE_PATH}")
 
# 真正include操作
include(test_include_with_macro)
 
message(STATUS "########## End include camke test_variable_macro\n")
 
# include后先验证一下普通变量值的变化（即修改是否生效）
message(STATUS "MY_LOCAL_VAR_7: ${MY_LOCAL_VAR}")
 
# include后访问一下被include模块module中的普通变量值
message(STATUS "HIS_VAR_1: ${HIS_VAR}")
 
# 调用include模块中定义的宏
test_macro("macro")  ## 调用宏操作
 
# 调用宏之后访问一下宏中定义的普通变量值
message(STATUS "macro_inner_val_2: ${macro_inner_val}")
 
# 调用宏之后验证一下宏中修改普通变量值的变化（即修改是否生效）
message(STATUS "HIS_VAR_3: ${HIS_VAR}")
message(STATUS "MY_LOCAL_VAR_8: ${MY_LOCAL_VAR}")
 
# 注销/取消/释放变量MY_LOCAL_VAR
unset(MY_LOCAL_VAR)
 
# 打印结束日志（include和宏）
message(STATUS "\n########## END_TEST_INCLUDE_MACRO\n")
 
# 打印开始日志（include和函数）
message(STATUS "\n########## BEGIN_TEST_INCLUDE_FUNC")
 
# 普通变量1个值
set(MY_LOCAL_VAR "AAA")
message(STATUS "MY_LOCAL_VAR_1: ${MY_LOCAL_VAR}")
 
# 在相同作用域修改普通变量值
set(MY_LOCAL_VAR "BBB")
message(STATUS "MY_LOCAL_VAR_2: ${MY_LOCAL_VAR}")
 
# include 子目录cmake中的camke test_variable_func.camke文件
message(STATUS "\n########## Begin include cmake test_variable_func")
 
# 同上
message(STATUS "CMAKE_MODULE_PATH_1: ${CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
message(STATUS "CMAKE_MODULE_PATH_2: ${CMAKE_MODULE_PATH}")
 
# 真正include操作
include(test_include_with_func)
 
message(STATUS "########## End include camke test_variable_func\n")
 
# include后先验证一下普通变量值的变化（即修改是否生效）
message(STATUS "MY_LOCAL_VAR_7: ${MY_LOCAL_VAR}")
 
# include后访问一下被include模块module中的普通变量值
message(STATUS "HER_VAR_1: ${HER_VAR}")
 
# 调用include模块中定义的函数
func("func_var")  ## 调用函数操作
 
# 调用函数之后访问一下函数中定义的普通变量值
message(STATUS "func_inner_val_2: ${func_inner_val}")
 
# 调用函数之后验证一下函数中修改普通变量值的变化（即修改是否生效）
message(STATUS "HER_VAR_3: ${HER_VAR}")
message(STATUS "MY_LOCAL_VAR_8: ${MY_LOCAL_VAR}")
 
# 调用函数之后访问一下函数中定义的普通变量值（注意与95行区别）
message(STATUS "MY_MODULE_FUN_VAR_2: ${MY_MODULE_FUN_VAR}")
 
# 打印结束日志（include和函数）
message(STATUS "\n########## END_TEST_INCLUDE_FUNC\n")