# 环境变量
- 1 定义环境变量
- 2 应用环境变量
    - 2.1 代码结构
    - 2.2 示例代码
    - 2.3 运行结果
- 3 小结

## 1 定义环境变量
```
set(ENV{<variable>} [<value>])
```
- ENV：环境变量标志性前缀
- variable：变量名称
- value：变量值

## 2 应用环境变量
### 2.1 代码结构
- 环境变量：根目录
- build：为CMake配置输出目录（在此例中即生成sln解决方案的地方）
- cmake_config.bat：执行CMake配置过程的脚本（双击直接运行）
- CMakeLists.txt：CMake脚本

### 2.2 示例代码
- 示例代码（CMakeLists.txt文件内容）
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
```
- cmake_config.bat
```
@echo off
set currentDir=%~dp0
set buildDir=%currentDir%
set cmakeOutputDir=%currentDir%\build
cmake -S %buildDir% -B %cmakeOutputDir% -G"Visual Studio 16 2019" -T v140 -A x64
pause
```

### 2.3 运行结果
本地安装VS版本：Visual Studio 2019（2019工具集）
CMake版本：3.24.1

```
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.19044.
-- The CXX compiler identification is MSVC 19.0.24245.0
-- The C compiler identification is MSVC 19.0.24245.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: D:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/bin/amd64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
--
########## BEGIN_TEST_ENV_VARIABLE
NOT DEFINED JAVA_HOME VARIABLES
CMAKE_PATH_1: F:/cmake
CMAKE_PATH_2: F:/cmake
CMAKE_FUNC_1: F:/cmake/dir
CMAKE_FUNC_2: F:/cmake/dir
NOT DEFINED CMAKE_FUNC_3 VARIABLES
CMAKE_PATH_3: F:/cmake
CMAKE_MACRO_1: F:/cmake/macro
CMAKE_MACRO_2: F:/cmake/macro
CMake Warning (dev) at CMakeLists.txt:98 (set):
  Only the first value argument is used when setting an environment variable.
  Argument 'F:/cmake/cmake2.txt' and later are unused.
This warning is for project developers.  Use -Wno-dev to suppress it.

CMAKE_FILE: F:/cmake/cmake1.txt
-- ########## END_TEST_ENV_VARIABLE

-- Configuring done
-- Generating done
-- Build files have been written to: E:/learning/learning-programming/modern-cmake/materials/环境变量/build
```

## 3 小结
- 设定或读取环境变量时，都通过ENV前缀来访问环境变量。

- 读取环境变量值时，要在ENV前加 **\$** 符号；但if判断是否定义时，不用加$符号。

- 设定环境变量，此命令只影响当前CMake进程，不影响调用CMake的进程，也不影响整个系统环境。

- 设定环境变量时，没有给变量赋值：
  如果有同名环境变量，此命令将清除已存在的同名环境变量；
  没有同名环境变量，此命令将被视为无效代码。
  如示例程序中第62行，将清除已存在的环境变量CMAKE_FUNC，这样导致输出结果第21行：NOT DEFINED CMAKE_FUNC_3 VARIABLES

- 设定环境变量时，变量值仅有一个。多余的值会被忽略，并且会提示警告信息。
  如示例程序中第98行，变量值为2个，结果第二个值被忽略，且会有CMake Warning打印到输出结果内容中。

- 环境变量，不存在作用域的区分。即函数、宏、子目录、包含模块中都一样，只会影响当前CMake进程。
  如示例程序中第52、88行，经过调用函数和宏的测试，其内部可以访问外部的环境变量；在其内部定义的环境变量，在外部也可以正常使用。

- 子目录与函数属于同类（有作用域区分），宏和包含模块属于同类（无作用域区分），因此，在此只用测试两种即可说明问题。

作者：kaizenly
出处：https://www.cnblogs.com/Braveliu/p/15617304.html
