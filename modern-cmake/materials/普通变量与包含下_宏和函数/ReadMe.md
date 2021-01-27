# 普通变量与include、macro
- 1 CMake普通变量与包含、宏示例
    - 1.1 代码目录结构
    - 1.2 根目录CMakeLists.txt
    - 1.3 根目录test_include_with_macro.cmake
    - 1.4 cmake子目录中的test_include_with_func.cmake
    - 1.5 执行CMake配置脚本
- 2 运行结果
    - 2.1 环境说明
    - 2.2 运行结果
    - 2.3 结论
        - 2.3.1 定义普通变量方式
        - 2.3.2 variable：表示普通变量名称；
        - 2.3.3 value：表示变量的值列表，value后面有三个点，表示可变参值，即变量的值可以为0或1或n个值
        - 2.3.4 PARENT_SCOPE：表示父作用域，若加此选项，特别明确定义或修改父作用域变量的值
        - 2.3.5 普通变量，在include模块中或macro宏中，与上层中应用完全相同，不存在作用域的差异。

## 1 CMake普通变量与包含、宏示例
### 1.1 代码目录结构
- 普通变量与包含_宏：为根目录（即父目录）
- build：为CMake配置输出目录（在此例中即生成sln解决方案的地方）
- cmake_config.bat：执行CMake配置过程的脚本（双击直接运行即可）
- CMakeLists.txt：根目录CMake脚本
- cmake：子目录，包含测试普通变量include与函数（function）的模块test_include_with_func.cmake文件。主要用于对比与宏macro的区别
- test_include_with_macro.cmake： 测试普通变量include并与宏（macro）的模块.cmake文件

1.2 根目录CMakeLists.txt
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
```

1.3 根目录test_include_with_macro.cmake
```
# 进入标志
message(STATUS "##### Enter test_include_with_macro.cmake file")
 
# 验证访问include()主目录CMakeList.txt中变量
message(STATUS "MY_LOCAL_VAR_3: ${MY_LOCAL_VAR}")
 
# 修改include()主目录CMakeList.txt中变量
set(MY_LOCAL_VAR "ccc")
message(STATUS "MY_LOCAL_VAR_4: ${MY_LOCAL_VAR}")
 
# 修改include()主目录CMakeList.txt中变量值加PARENT_SCOPE选项
# set(MY_LOCAL_VAR "ddd" PARENT_SCOPE) # CMake Warning 警告:主目录不被视为父目录，即相当于同作用域
 
# 在此cmake文件中定义变量
set(HIS_VAR "123")
 
# 定义宏
macro(test_macro M_VAR)
    # 调用宏
    message(STATUS "##### Start Call macro")
    
    # 打印宏参数值
    message(STATUS "M_VAR: ${M_VAR}")
    
    # 宏内部定义变量
    set(macro_inner_val "C++")
    # 打印宏内部变量值
    message(STATUS "macro_inner_val_1: ${macro_inner_val}")
    
    # 修改此cmake文件中变量值
    set(HIS_VAR "456")
    message(STATUS "HIS_VAR_2: ${HIS_VAR}")
    
    # 打印include()主目录CMakeList.txt中变量
    message(STATUS "MY_LOCAL_VAR_5: ${MY_LOCAL_VAR}")
    
    # 修改include()主目录CMakeList.txt中变量值
    set(MY_LOCAL_VAR "ddd")
    message(STATUS "MY_LOCAL_VAR_6: ${MY_LOCAL_VAR}")
    
    # 修改include()主目录CMakeList.txt中变量值加PARENT_SCOPE选项
    # set(MY_LOCAL_VAR "eee" PARENT_SCOPE) # CMake Warning 警告:主目录不被视为父目录，即与宏也同作用域
    
    message(STATUS "##### Stop Call macro")
endmacro(test_macro)
 
message(STATUS "##### Leave test_include_with_macro.cmake file")
```

1.4 cmake子目录中的test_include_with_func.cmake
```
# 进入标志
message(STATUS "##### Enter test_include_with_func.cmake file")
 
# 验证访问include()主目录CMakeList.txt中变量
message(STATUS "MY_LOCAL_VAR_3: ${MY_LOCAL_VAR}")
 
# 修改include()主目录CMakeList.txt中变量
set(MY_LOCAL_VAR "CCC")
message(STATUS "MY_LOCAL_VAR_4: ${MY_LOCAL_VAR}")
 
# 修改include()主目录CMakeList.txt中变量值加PARENT_SCOPE选项
# set(MY_LOCAL_VAR "ddd" PARENT_SCOPE) # CMake Warning 警告:主目录不被视为父目录，即相当于同作用域
 
# 在此cmake文件中定义变量
set(HER_VAR "red")
 
# 定义函数
function(func F_VAR)
    # 调用函数
    message(STATUS "##### Start Call Func")
    
    # 打印函数参数值
    message(STATUS "F_VAR: ${F_VAR}")
    
    # 函数内部定义变量
    set(func_inner_val "JAVA")
    # 打印函数内部变量值
    message(STATUS "func_inner_val_1: ${func_inner_val}")
    
    # 修改此cmake文件中变量值
    set(HER_VAR "black")
    message(STATUS "HER_VAR_2: ${HER_VAR}")
    
    # 打印include()主目录CMakeList.txt中变量
    message(STATUS "MY_LOCAL_VAR_5: ${MY_LOCAL_VAR}")
    
    # 修改include()主目录CMakeList.txt中变量值不加PARENT_SCOPE选项
    set(MY_LOCAL_VAR "DDD")
    message(STATUS "MY_LOCAL_VAR_6: ${MY_LOCAL_VAR}")
    
    # 定义变量加PARENT_SCOPE选项
    set(MY_MODULE_FUN_VAR "I am a module inner func variable" PARENT_SCOPE) # 正常无警告。根目录被视为父目录，即函数与其不属于同一作用域
    message(STATUS "MY_MODULE_FUN_VAR_1: ${MY_MODULE_FUN_VAR}")
    
    message(STATUS "##### Stop Call Func")
endfunction()
 
message(STATUS "##### Leave test_include_with_func.cmake file")
```

1.5 执行CMake配置脚本
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
########## BEGIN_TEST_INCLUDE_MACRO
-- MY_LOCAL_VAR_1: aaa
-- MY_LOCAL_VAR_2: bbb
--
########## Begin include camke test_variable_macro
-- CMAKE_MODULE_PATH_1:
-- CMAKE_MODULE_PATH_2: F:/learn_cmake
-- ##### Enter test_include_with_macro.cmake file
-- MY_LOCAL_VAR_3: bbb
-- MY_LOCAL_VAR_4: ccc
-- ##### Leave test_include_with_macro.cmake file
-- ########## End include camke test_variable_macro
 
-- MY_LOCAL_VAR_7: ccc
-- HIS_VAR_1: 123
-- ##### Start Call macro
-- M_VAR: macro
-- macro_inner_val_1: C++
-- HIS_VAR_2: 456
-- MY_LOCAL_VAR_5: ccc
-- MY_LOCAL_VAR_6: ddd
-- ##### Stop Call macro
-- macro_inner_val_2: C++
-- HIS_VAR_3: 456
-- MY_LOCAL_VAR_8: ddd
--
########## END_TEST_INCLUDE_MACRO
 
--
########## BEGIN_TEST_INCLUDE_FUNC
-- MY_LOCAL_VAR_1: AAA
-- MY_LOCAL_VAR_2: BBB
--
########## Begin include cmake test_variable_func
-- CMAKE_MODULE_PATH_1: F:/learn_cmake
-- CMAKE_MODULE_PATH_2: F:/learn_cmake;F:/learn_cmake/cmake
-- ##### Enter test_include_with_func.cmake file
-- MY_LOCAL_VAR_3: BBB
-- MY_LOCAL_VAR_4: CCC
-- ##### Leave test_include_with_func.cmake file
-- ########## End include camke test_variable_func
 
-- MY_LOCAL_VAR_7: CCC
-- HER_VAR_1: red
-- ##### Start Call Func
-- F_VAR: func_var
-- func_inner_val_1: JAVA
-- HER_VAR_2: black
-- MY_LOCAL_VAR_5: CCC
-- MY_LOCAL_VAR_6: DDD
-- MY_MODULE_FUN_VAR_1:
-- ##### Stop Call Func
-- func_inner_val_2:
-- HER_VAR_3: red
-- MY_LOCAL_VAR_8: CCC
-- MY_MODULE_FUN_VAR_2: I am a module inner func variable
--
########## END_TEST_INCLUDE_FUNC
 
-- Configuring done
-- Generating done
-- Build files have been written to: F:/learn_cmake/build
请按任意键继续. . .
```

## 2.3 结论
### 2.3.1 定义普通变量方式
```
set(<variable> <value>... [PARENT_SCOPE])
```

### 2.3.2 variable：表示普通变量名称；
### 2.3.3 value：表示变量的值列表，value后面有三个点，表示可变参值，即变量的值可以为0或1或n个值
具体示例及说明可参见随笔加深理解与体会。

### 2.3.4 PARENT_SCOPE：表示父作用域，若加此选项，特别明确定义或修改父作用域变量的值
### 2.3.5 普通变量，在include模块中或macro宏中，与上层中应用完全相同，不存在作用域的差异。
- 换句话表述：如果CMake语法—普通变量与函数和 CMake语法—普通变量与子目录的读者，经过清晰对比，这里可理解为与Function和Subdirectory的情况刚好相反。

下面从示例程序出发，再结合输出结果分析（为了方便理解，上文示例程序分别用1.2、1.3、1.4代表；输出结果用2.2代表）：

- 普通变量，在include模块中与Macro宏中应用，与上层中完全相同。
```
如示例1.2中第23行、1.3中第8、38行、1.4中的8行，结合输出2.2中第17、24、35、53行结果可知，形散神不散，一气呵成。

反倒是1.3中第12、42行因为添加PARENT_SCOPE选项，被CMake发出警告，提示“画蛇添足”，有多此一举之嫌。
```
- 在include模块和Macro宏中定义的普通变量，在上层中也可以正常应用，不存在差异。
```
如示例1.3中第15、26行、1.4中15行，结合输出2.2中第29、32、58行结果可知，浑然一体，熔于一炉。

与之形成鲜明对比如示例1.4中第26、31行，结果输出2.2中第67、68行结果可知，函数中就存在作用域的区分和差异。

另外，示例1.4中第42行，在函数中定义父作用域变量，加PARENT_SCOPE选项，无任何警告，且输出2.2中第70行的确有效果。
```
- include和macro与.cmake模块文件存放位置无关。
```
如示例1.3程序存放在根目录，示例1.4存放在cmake文件夹中，include后结果是完全相同的效果。
```
- 普通变量，在include和macro中应用方式统一（无隔阂），在function和subdirectory中应用方式一致（分伯仲）。
```
可结合示例中函数func和宏test_macro仔细体会一下彼此的差异和个性。

比如1.3中第31行对变量的修改，1.4中第31行对变量的修改，从输出2.2中第38、68行结果可知，两者差异还是很清晰。
```
- unset 注销、取消、释放变量
```
unset(<variable> PARENT_SCOPE)
variable：设置要清除的变量名
```
- PARENT_SCOPE（可选）：若清除的是父作用域中的变量，需要加此选项；否则，无效果。可参照set逆向理解与体会。

- 尤其注意示例1.2中第56行，为了前后两种测试更有可比性，第46行对同一个变量进行”清零”处理。

- 参考与引用

[cmake中的include指令（.cmake文件/MACRO宏/function函数）](https://yngzmiao.blog.csdn.net/article/details/102677143)
[CMake语法—普通变量与包含、宏（Normal Variable And Include、Macro）](https://www.cnblogs.com/Braveliu/p/15609273.html)