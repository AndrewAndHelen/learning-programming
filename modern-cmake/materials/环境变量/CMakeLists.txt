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
message(STATUS "\n########## BEGIN_TEST_ENV_VARIABLE")
 
# 判断JAVA_HOME变量是否定义
if(DEFINED ENV{JAVA_HOME})
    message("JAVA_HOME: $ENV{JAVA_HOME}")
else()
    message("NOT DEFINED JAVA_HOME VARIABLES")
endif()
 
# 定义环境变量
set(ENV{CMAKE_PATH} "F:/cmake")
 
# 判断CMAKE_PATH环境变量是否定义
if(DEFINED ENV{CMAKE_PATH})
    message("CMAKE_PATH_1: $ENV{CMAKE_PATH}")
else()
    message("NOT DEFINED CMAKE_PATH VARIABLES")
endif()
 
# 定义测试函数，在函数中新定义环境变量
function(test_env_variable)
    # 访问环境变量CMAKE_PATH
    message("CMAKE_PATH_2: $ENV{CMAKE_PATH}")
    
    # 函数内定义环境变量
    set(ENV{CMAKE_FUNC} "F:/cmake/dir")
 
    # 判断CMAKE_FUNC环境变量是否定义
    if(DEFINED ENV{CMAKE_FUNC})
        message("CMAKE_FUNC_1: $ENV{CMAKE_FUNC}")
    else()
        message("NOT DEFINED CMAKE_FUNC_1 VARIABLES")
    endif()
endfunction()
 
# 调用函数
test_env_variable()
 
# 判断CMAKE_FUNC环境变量是否定义
if(DEFINED ENV{CMAKE_FUNC})
    message("CMAKE_FUNC_2: $ENV{CMAKE_FUNC}")
else()
    message("NOT DEFINED CMAKE_FUNC_2 VARIABLES")
endif()
 
# 如果没有参数值
set(ENV{CMAKE_FUNC})
 
# 判断CMAKE_FUNC环境变量是否定义
if(DEFINED ENV{CMAKE_FUNC})
    message("CMAKE_FUNC_3: $ENV{CMAKE_FUNC}")
else()
    message("NOT DEFINED CMAKE_FUNC_3 VARIABLES")
endif()
 
# 定义测试宏，在函数中新定义环境变量
macro(test_env_var)
    # 访问环境变量CMAKE_PATH
    message("CMAKE_PATH_3: $ENV{CMAKE_PATH}")
    
    # 宏内定义环境变量
    set(ENV{CMAKE_MACRO} "F:/cmake/macro")
 
    # 判断CMAKE_MACRO环境变量是否定义
    if(DEFINED ENV{CMAKE_MACRO})
        message("CMAKE_MACRO_1: $ENV{CMAKE_MACRO}")
    else()
        message("NOT DEFINED CMAKE_MACRO_1 VARIABLES")
    endif()
endmacro()
 
# 调用宏
test_env_var()
 
# 判断CMAKE_MACRO环境变量是否定义
if(DEFINED ENV{CMAKE_MACRO})
    message("CMAKE_MACRO_2: $ENV{CMAKE_MACRO}")
else()
    message("NOT DEFINED CMAKE_MACRO_2 VARIABLES")
endif()
 
# 如果多个参数值
set(ENV{CMAKE_FILE} "F:/cmake/cmake1.txt" "F:/cmake/cmake2.txt")
 
# 判断CMAKE_FILE环境变量是否定义
if(DEFINED ENV{CMAKE_FILE})
    message("CMAKE_FILE: $ENV{CMAKE_FILE}")
else()
    message("NOT DEFINED CMAKE_FILE VARIABLES")
endif()
 
# 打印结束日志
message(STATUS "########## END_TEST_ENV_VARIABLE\n")