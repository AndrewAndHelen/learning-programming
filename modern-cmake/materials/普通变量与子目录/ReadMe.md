# 普通变量与子目录
- 1 CMake普通变量与子目录示例
    - 1.1 代码目录结构
    - 1.2 父目录CMakeLists.txt
    - 1.3 src子目录CMakeLists.txt
    - 1.4 src_parent_scope子目录CMakeLists.txt
    - 1.5 执行CMake配置脚本
- 2 运行结果
    - 2.1 环境说明
    - 2.2 运行结果
    - 2.3 结论
        - 2.3.1 定义普通变量方式
        - 2.3.2 variable：表示普通变量名称；
        - 2.3.3 value：表示变量的值列表，value后面有三个点，表示变量的值可以为0或1或n个值
        - 2.3.4 PARENT_SCOPE：表示父作用域，若加此选项，特别明确定义或修改父作用域变量的值
        - 2.3.5 普通变量，在子目录subdirectory作用域中，会拷贝一份父作用域的所有普通变量（即在被add子目录内可访问父作用域内的普通变量）

## 1 CMake普通变量与子目录示例
### 1.1 代码目录结构

### 1.2 父目录CMakeLists.txt
```
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
```

### 1.3 src子目录CMakeLists.txt
```
cmake_minimum_required(VERSION 3.18)
 
# 打印进入日志
message(STATUS "########## Enter src subdirectory")
 
# 修改父作用域普通变量值
# 修改前打印
message(STATUS "MY_LOCAL_VAR_3: ${MY_LOCAL_VAR}")
# 修改动作
set(MY_LOCAL_VAR "I am a local variable by modify in src subdirectory")
# 修改后打印
message(STATUS "MY_LOCAL_VAR_4: ${MY_LOCAL_VAR}")
 
# 子目录作用域内定义普通变量，不加选项PARENT_SCOPE
set(MY_SUB_DIR_VAR "I am a src subdirectory inner variable")
# 子目录作用域打印定义变量值
message(STATUS "MY_SUB_DIR_VAR_1: ${MY_SUB_DIR_VAR}")
 
# 打印离开日志
message(STATUS "########## Leave src subdirectory")
```
### 1.4 src_parent_scope子目录CMakeLists.txt
```
cmake_minimum_required(VERSION 3.18)
 
# 打印进入日志
message(STATUS "##########Enter src_parent_scope subdirectory")
 
# 修改父作用域普通变量值
# 修改前打印
message(STATUS "MY_LOCAL_VAR_5: ${MY_LOCAL_VAR}")
# 修改动作
set(MY_LOCAL_VAR "I am a local variable by modify in src_parent_scope subdirectory" PARENT_SCOPE)
# 修改后打印
message(STATUS "MY_LOCAL_VAR_6: ${MY_LOCAL_VAR}")
 
# 子目录作用域内定义普通变量，加选项PARENT_SCOPE
set(MY_SUB_DIR_VAR "I am a src_parent_scope subdirectory inner variable" PARENT_SCOPE)
# 子目录作用域打印定义变量值
message(STATUS "MY_SUB_DIR_VAR_2: ${MY_SUB_DIR_VAR}")
 
# 打印离开日志
message(STATUS "##########Leave src_parent_scope subdirectory")
```

### 1.5 执行CMake配置脚本
```
@echo off
set currentDir=%~dp0
set buildDir=%currentDir%
set cmakeOutputDir=%currentDir%\build
cmake -S %buildDir% -B %cmakeOutputDir% -G"Visual Studio 16 2019" -T v140 -A x64
pause
```

## 2 运行结果
### 2.1 环境说明
本地安装VS版本：Visual Studio 2019（2019工具集）
CMake版本：3.24.1

### 2.2 运行结果
```
-- Selecting Windows SDK version 10.0.18362.0 to target Windows 10.0.17763.
-- The CXX compiler identification is MSVC 19.0.24245.0
-- The C compiler identification is MSVC 19.0.24245.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
--
########## BEGIN_TEST
-- MY_LOCAL_ZERO_VAR:
-- MY_LOCAL_VAR_1: I am a local variable
-- MY_LOCAL_VAR_2: I am a local variable by modify same scope
-- MY_LOCAL_LIST_VAR: kaizen;baidu;C++;developer
--
########## Begin add_subdirectory src
-- ########## Enter src subdirectory
-- MY_LOCAL_VAR_3: I am a local variable by modify same scope
-- MY_LOCAL_VAR_4: I am a local variable by modify in src subdirectory
-- MY_SUB_DIR_VAR_1: I am a src subdirectory inner variable
-- ########## Leave src subdirectory
-- ########## End add_subdirectory src
-- MY_LOCAL_VAR_7: I am a local variable by modify same scope
-- MY_SUB_DIR_VAR_3:
--
########## Begin add_subdirectory src_parent_scope
-- ##########Enter src_parent_scope subdirectory
-- MY_LOCAL_VAR_5: I am a local variable by modify same scope
-- MY_LOCAL_VAR_6: I am a local variable by modify same scope
-- MY_SUB_DIR_VAR_2:
-- ##########Leave src_parent_scope subdirectory
-- ########## End add_subdirectory src_parent_scope
-- MY_LOCAL_VAR_8: I am a local variable by modify in src_parent_scope subdirectory
-- MY_SUB_DIR_VAR_4: I am a src_parent_scope subdirectory inner variable
--
########## END_TEST
 
-- Configuring done
-- Generating done
-- Build files have been written to: F:/learn_cmake/build
请按任意键继续. . .
```
## 2.3 结论
2.3.1 定义普通变量方式
```
set(<variable> <value>... [PARENT_SCOPE])
```
### 2.3.2 variable：表示普通变量名称；
### 2.3.3 value：表示变量的值列表，value后面有三个点，表示变量的值可以为0或1或n个值
- 示例程序中第19行：定义普通变量，赋值为0个值，可理解为占位符。值为空，打印结果也为空，参见运行结果输出第16行。
- 示例程序中第23行：定义普通变量，赋值为1个值，即字符串中内容。参见运行结果输出第17、18行打印内容。
- 示例程序中第31行：定义普通变量，赋值为4个值，当多个值时，将来会用分号拼接起来为一个字符串。参见运行结果输出第19行打印内容。

### 2.3.4 PARENT_SCOPE：表示父作用域，若加此选项，特别明确定义或修改父作用域变量的值
### 2.3.5 普通变量，在子目录subdirectory作用域中，会拷贝一份父作用域的所有普通变量（即在被add子目录内可访问父作用域内的普通变量）
- 从示例程序出发，再结合输出结果分析可知：普通变量在函数作用域与子目录作用域中，实际运行效果完全相同。

作者：kaizenly
出处：https://www.cnblogs.com/Braveliu/p/15604270.html