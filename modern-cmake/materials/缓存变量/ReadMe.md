# 缓存变量
- 1 CMake缓存变量
- 2 定义缓存变量
    - 2.1 定义格式
    - 2.2 定义示例代码
    - 2.3 运行结果
    - 2.4 小结
- 3 CMakeCache.txt文件
- 4 缓存变量在函数(function)、宏(macro)、子目录(subdirectory)、包含模块(include)中应用
    - 4.1 应用示例
    - 4.2 运行结果
    - 4.3 小结
- 5 缓存变量与普通变量相互转换
    - 5.1 关系示例
    - 5.2 运行结果
    - 5.3 小结
- 6 通过终端预设缓存变量值
    - 6.1 应用示例
    - 6.2 运行结果
    - 6.3 小结
- 7 缓存变量应用注意事项
    - 7.1 缓存变量，本质是全局变量
    - 7.2 缓存变量，都会存储在CMakeCache.txt文件中
    - 7.3 缓存变量发生问题，一定记得先删除build目录下的CMakeCache.txt文件，然后重新配置项目

## 1 CMake缓存变量
- Normal Variable，普通变量，相当于一个局部变量。在同一个CMake工程中使用，会有作用域限制或区分。

- Cache Variable，缓存变量，相当于一个全局变量。在同一个CMake工程中任何地方都可以使用。

## 2 定义缓存变量
### 2.1 定义格式
```
set(<variable> <value>... CACHE <type> <docstring> [FORCE])
```

- variable：变量名称
- value：变量值列表
- CACHE：cache变量的标志
- type：变量类型，取决于变量的值。类型分为：BOOL、FILEPATH、PATH、STRING、INTERNAL
- docstring：必须是字符串，作为变量概要说明
- FORCE：强制选项，强制修改变量值

### 2.2 定义示例代码
- 代码结构见 ./materials/缓存变量/cache1
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
message(STATUS "\n########## BEGIN_TEST_CACHE_VARIABLE")
 
# 定义一个STRIING类型缓存变量(不加FORCE选项)
set(MY_GLOBAL_VAR_STRING_NOFORCE "abcdef" CACHE STRING "定义一个STRING缓存变量")
message("MY_GLOBAL_VAR_STRING_NOFORCE: ${MY_GLOBAL_VAR_STRING_NOFORCE}")
 
# 定义一个STRIING类型缓存变量(加FORCE选项)
set(MY_GLOBAL_VAR_STRING "abc" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING: ${MY_GLOBAL_VAR_STRING}")
 
# 定义一个INTERNAL类型缓存变量
set(MY_GLOBAL_VAR_INTERNAL "aaabbb" CACHE INTERNAL "定义一个INTERNAL类型的缓存变量")
message("MY_GLOBAL_VAR_INTERNAL: ${MY_GLOBAL_VAR_INTERNAL}")
 
# 定义一个BOOL类型缓存变量
set(MY_GLOBAL_VAR_BOOL OFF CACHE BOOL "定义一个BOOL类型的缓存变量")
message("MY_GLOBAL_VAR_BOOL: ${MY_GLOBAL_VAR_BOOL}")
 
# 定义一个FILEPATH类型缓存变量
set(MY_GLOBAL_VAR_FILEPATH "F:/learn_cmake/CMakeLists.txt" CACHE FILEPATH "定义一个FILEPATH类型的缓存变量")
message("MY_GLOBAL_VAR_FILEPATH: ${MY_GLOBAL_VAR_FILEPATH}")
 
# 定义一个PATH类型缓存变量
set(MY_GLOBAL_VAR_PATH "F:/learn_cmake" CACHE PATH "定义一个PATH类型的缓存变量")
message("MY_GLOBAL_VAR_PATH: ${MY_GLOBAL_VAR_PATH}")
 
# 缓存变量3个值(验证值列表)
set(MY_GLOBAL_VAR_THRESS "aaa" "bbb" "ccc" CACHE STRING "定义一个缓存变量（3个值）")
message("MY_GLOBAL_VAR_THRESS: ${MY_GLOBAL_VAR_THRESS}")
 
# 修改STRING类型缓存变量(不加FORCE选项)
set(MY_GLOBAL_VAR_STRING "modifyabc" CACHE STRING "修改STRING缓存变量")
message("MY_GLOBAL_VAR_STRING: ${MY_GLOBAL_VAR_STRING}")
 
# 修改STRING类型缓存变量(加FORCE选项)
set(MY_GLOBAL_VAR_STRING "modifyabcforce" CACHE STRING "修改STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING: ${MY_GLOBAL_VAR_STRING}")
 
# 打印结束日志
message(STATUS "########## END_TEST_CACHE_VARIABLE\n")
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
- 本地环境
- 输出结果
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
########## BEGIN_TEST_CACHE_VARIABLE
MY_GLOBAL_VAR_STRING_NOFORCE: abcdef
MY_GLOBAL_VAR_STRING: abc
MY_GLOBAL_VAR_INTERNAL: aaabbb
MY_GLOBAL_VAR_BOOL: OFF
MY_GLOBAL_VAR_FILEPATH: F:/learn_cmake/CMakeLists.txt
MY_GLOBAL_VAR_PATH: F:/learn_cmake
MY_GLOBAL_VAR_THRESS: aaa;bbb;ccc
MY_GLOBAL_VAR_STRING: abc
MY_GLOBAL_VAR_STRING: modifyabcforce
-- ########## END_TEST_CACHE_VARIABLE
 
-- Configuring done
-- Generating done
-- Build files have been written to: F:/learn_cmake/build
请按任意键继续. . .
```

### 2.4 小结
```
1. 定义缓存变量时，可以不加FORCE选项：

   示例程序中第19行添加FORCE，第23行未加FORCE选项，结果没有发现两者差异。后来其它类型都没有加FORCE选项。

2. 修改缓存变量时，一定要加FORCE选项，否则修改无效：

   示例程序中第47行，试图修改缓存变量的值，没有添加FORCE选项，结合修改运行结果第23行可知，修改无效。

   相反，程序中第51行，企图修改值时添加FORCE选项，结合修改运行结果第24行可知，修改生效。

3. Cache变量都会保存在CMakeCache.txt文件中。

   不论定义或修改缓存变量时，建议都加上FORCE选项。结合1、2项所得。
```

## 3 CMakeCache.txt文件
```
CMake中的缓存变量都会保存在CMakeCache.txt文件中。

至于为何这样干，可以逆向理解，如若不保存，想作为全局变量根本没法实现。

限于篇幅，在此不展示CMakeCache.txt中的内容。因为其中包括很多默认的缓存变量，比较多。可在本地自行运行示例程序后查看即可。
```

## 4 缓存变量在函数(function)、宏(macro)、子目录(subdirectory)、包含模块(include)中应用
### 4.1 应用示例
- 代码结构 ./materials/缓存变量/cache2
- 父目录CMakeLists.txt
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
message(STATUS "\n########## BEGIN_TEST_CACHE_VARIABLE")
 
# 定义一个STRIING类型缓存变量
set(MY_GLOBAL_VAR_STRING "abcdef" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING_1: ${MY_GLOBAL_VAR_STRING}")
 
function(test_cache_func)
    # 访问函数外定义的缓存变量
    message("MY_GLOBAL_VAR_STRING_2: ${MY_GLOBAL_VAR_STRING}")
    
    # 修改函数外定义的缓存变量
    set(MY_GLOBAL_VAR_STRING "abcdef modify by func" CACHE STRING "修改缓存变量" FORCE)
 
    # 访问修改后的函数外缓存变量
    message("MY_GLOBAL_VAR_STRING_3: ${MY_GLOBAL_VAR_STRING}")
    
    # 在函数中新定义一个STRIING类型缓存变量
    set(MY_GLOBAL_VAR_STRING_FUNC "I am a func inner cache variable" CACHE STRING "定义一个STRING缓存变量" FORCE)
    message("MY_GLOBAL_VAR_STRING_FUNC_1: ${MY_GLOBAL_VAR_STRING_FUNC}")
 
endfunction()
 
message(STATUS "\n##### BEGIN_TEST_FUNC_CACHE_VAR")
 
# 调用函数
test_cache_func()
 
# 访问缓存变量的值
message("MY_GLOBAL_VAR_STRING_4: ${MY_GLOBAL_VAR_STRING}")
message("MY_GLOBAL_VAR_STRING_FUNC_2: ${MY_GLOBAL_VAR_STRING_FUNC}")
 
message(STATUS "##### BEGIN_TEST_FUNC_CACHE_VAR")
 
macro(test_cache_macro)
    # 访问宏外定义的缓存变量
    message("MY_GLOBAL_VAR_STRING_5: ${MY_GLOBAL_VAR_STRING}")
    
    # 修改宏外定义的缓存变量
    set(MY_GLOBAL_VAR_STRING "abcdef modify by macro" CACHE STRING "修改缓存变量" FORCE)
 
    # 访问修改后的宏外缓存变量
    message("MY_GLOBAL_VAR_STRING_6: ${MY_GLOBAL_VAR_STRING}")
    
    # 在宏中新定义一个STRIING类型缓存变量
    set(MY_GLOBAL_VAR_STRING_MACRO "I am a macro inner cache variable" CACHE STRING "定义一个STRING缓存变量" FORCE)
    message("MY_GLOBAL_VAR_STRING_MACRO_1: ${MY_GLOBAL_VAR_STRING_MACRO}")
endmacro()
 
message(STATUS "\n##### BEGIN_TEST_MACRO_CACHE_VAR")
 
# 调用宏
test_cache_macro()
 
# 访问缓存变量的值
message("MY_GLOBAL_VAR_STRING_7: ${MY_GLOBAL_VAR_STRING}")
message("MY_GLOBAL_VAR_STRING_MACRO_2: ${MY_GLOBAL_VAR_STRING_MACRO}")
 
message(STATUS "##### END_TEST_MACRO_CACHE_VAR")
 
message(STATUS "\n##### BEGIN_TEST_SUBDIR_CACHE_VAR")
 
add_subdirectory(src)
 
# 访问缓存变量的值
message("MY_GLOBAL_VAR_STRING_10: ${MY_GLOBAL_VAR_STRING}")
message("MY_GLOBAL_VAR_STRING_SUBDIR_2: ${MY_GLOBAL_VAR_STRING_SUBDIR}")
 
message(STATUS "##### END_TEST_SUBDIR_CACHE_VAR")
 
message(STATUS "\n##### BEGIN_TEST_INCLUDE_CACHE_VAR")
 
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
include(test_include_cache)
 
# 访问缓存变量的值
message("MY_GLOBAL_VAR_STRING_13: ${MY_GLOBAL_VAR_STRING}")
message("MY_GLOBAL_VAR_STRING_INCLUDE_2: ${MY_GLOBAL_VAR_STRING_INCLUDE}")
 
message(STATUS "##### END_TEST_INCLUDE_CACHE_VAR")
 
# 打印结束日志
message(STATUS "########## END_TEST_CACHE_VARIABLE\n")
```

- src子目录CMakeLists.txt
```
cmake_minimum_required(VERSION 3.18)
 
# 访问父目录的缓存变量
message("MY_GLOBAL_VAR_STRING_8: ${MY_GLOBAL_VAR_STRING}")
 
# 修改父目录的缓存变量
set(MY_GLOBAL_VAR_STRING "abcdef modify by subdir" CACHE STRING "修改缓存变量" FORCE)
 
# 访问修改后的父目录缓存变量
message("MY_GLOBAL_VAR_STRING_9: ${MY_GLOBAL_VAR_STRING}")
 
# 在子目录中新定义一个STRIING类型缓存变量
set(MY_GLOBAL_VAR_STRING_SUBDIR "I am a sub directory inner cache variable" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING_SUBDIR_1: ${MY_GLOBAL_VAR_STRING_SUBDIR}")
```

- cmake子目录test_include_cache.cmake
```
cmake_minimum_required(VERSION 3.18)
 
# 访问调用者的缓存变量
message("MY_GLOBAL_VAR_STRING_11: ${MY_GLOBAL_VAR_STRING}")
 
# 修改调用者的缓存变量
set(MY_GLOBAL_VAR_STRING "abcdef modify by include mudule" CACHE STRING "修改缓存变量" FORCE)
 
# 访问修改后的调用者缓存变量
message("MY_GLOBAL_VAR_STRING_12: ${MY_GLOBAL_VAR_STRING}")
 
# 在此模块中新定义一个STRIING类型缓存变量
set(MY_GLOBAL_VAR_STRING_INCLUDE "I am a include module inner cache variable" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING_INCLUDE_1: ${MY_GLOBAL_VAR_STRING_INCLUDE}")
```

- cmake_config.bat
与上文示例一样，不做赘述。

### 4.2 运行结果
- 本地环境
同上文示例中的环境，不做赘述。

- 输出结果
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
########## BEGIN_TEST_CACHE_VARIABLE
MY_GLOBAL_VAR_STRING_1: abcdef
--
##### BEGIN_TEST_FUNC_CACHE_VAR
MY_GLOBAL_VAR_STRING_2: abcdef
MY_GLOBAL_VAR_STRING_3: abcdef modify by func
MY_GLOBAL_VAR_STRING_FUNC_1: I am a func inner cache variable
MY_GLOBAL_VAR_STRING_4: abcdef modify by func
MY_GLOBAL_VAR_STRING_FUNC_2: I am a func inner cache variable
-- ##### BEGIN_TEST_FUNC_CACHE_VAR
--
##### BEGIN_TEST_MACRO_CACHE_VAR
MY_GLOBAL_VAR_STRING_5: abcdef modify by func
MY_GLOBAL_VAR_STRING_6: abcdef modify by macro
MY_GLOBAL_VAR_STRING_MACRO_1: I am a macro inner cache variable
MY_GLOBAL_VAR_STRING_7: abcdef modify by macro
MY_GLOBAL_VAR_STRING_MACRO_2: I am a macro inner cache variable
-- ##### END_TEST_MACRO_CACHE_VAR
--
##### BEGIN_TEST_SUBDIR_CACHE_VAR
MY_GLOBAL_VAR_STRING_8: abcdef modify by macro
MY_GLOBAL_VAR_STRING_9: abcdef modify by subdir
MY_GLOBAL_VAR_STRING_SUBDIR_1: I am a sub directory inner cache variable
MY_GLOBAL_VAR_STRING_10: abcdef modify by subdir
MY_GLOBAL_VAR_STRING_SUBDIR_2: I am a sub directory inner cache variable
-- ##### END_TEST_SUBDIR_CACHE_VAR
--
##### BEGIN_TEST_INCLUDE_CACHE_VAR
MY_GLOBAL_VAR_STRING_11: abcdef modify by subdir
MY_GLOBAL_VAR_STRING_12: abcdef modify by include mudule
MY_GLOBAL_VAR_STRING_INCLUDE_1: I am a include module inner cache variable
MY_GLOBAL_VAR_STRING_13: abcdef modify by include mudule
MY_GLOBAL_VAR_STRING_INCLUDE_2: I am a include module inner cache variable
-- ##### END_TEST_INCLUDE_CACHE_VAR
-- ########## END_TEST_CACHE_VARIABLE
 
-- Configuring done
-- Generating done
-- Build files have been written to: F:/learn_cmake/build
请按任意键继续. . .
```
### 4.3 小结
缓存变量在函数、宏、子目录、包含模块中使用，没有任何差别。

全局变量，全工程项目使用方式相同。可以理解与C、C++全局变量的“格局”一样。

## 5 缓存变量与普通变量相互转换
### 5.1 关系示例
- 代码结构 见./materials/缓存变量/cache3
- 示例程序（CMakeLists.txt）
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
message(STATUS "\n########## BEGIN_TEST_CACHE_VARIABLE")
 
# 1.1 定义一个STRIING类型缓存变量
set(MY_GLOBAL_VAR_STRING "abcdef" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING_1: ${MY_GLOBAL_VAR_STRING}")
 
function(test_normal_var suffix)
  # 访问
  message("MY_GLOBAL_VAR_STRING_3: ${MY_GLOBAL_VAR_STRING}")
  # 修改
  set(MY_GLOBAL_VAR_STRING "modify by in normal func inner :: ${suffix}" PARENT_SCOPE)
  message("MY_GLOBAL_VAR_STRING_4: ${MY_GLOBAL_VAR_STRING}")
 
  # 定义新的普通变量
  set(MY_LOCAL_VAR_NORMAL_FUNC "my local var define in normal func :: ${suffix}")
  message("MY_LOCAL_VAR_NORMAL_FUNC_1: ${MY_LOCAL_VAR_NORMAL_FUNC}")
endfunction()
 
function(test_global_var suffix)
  # 访问
  message("MY_GLOBAL_VAR_STRING_5: ${MY_GLOBAL_VAR_STRING}")
  # 修改
  set(MY_GLOBAL_VAR_STRING "modify by in global func inner :: ${suffix}" CACHE STRING "修改STRING缓存变量" FORCE)
  message("MY_GLOBAL_VAR_STRING_6: ${MY_GLOBAL_VAR_STRING}")
 
  # 定义新的缓存变量
  set(MY_GLOBAL_VAR_GLOBAL_FUNC "my global var define in global func :: ${suffix}" CACHE STRING "定义一个STRING缓存变量" FORCE)
  message("MY_GLOBAL_VAR_GLOBAL_FUNC_1: ${MY_GLOBAL_VAR_GLOBAL_FUNC}")
endfunction()
 
message("\n###### BEGIN_Test_From_Cache_To_Normal\n")
 
# 1.2 定义一个同名的普通变量（全局变量沦为普通变量示例）
set(MY_GLOBAL_VAR_STRING "I am a normal variable but name same as cache variable")
message("MY_GLOBAL_VAR_STRING_2: ${MY_GLOBAL_VAR_STRING}\n")
 
test_normal_var("aaa")
message("\nMY_GLOBAL_VAR_STRING_7: ${MY_GLOBAL_VAR_STRING}")
message("MY_LOCAL_VAR_NORMAL_FUNC_2: ${MY_LOCAL_VAR_NORMAL_FUNC}\n")
 
test_global_var("bbb")
message("\nMY_GLOBAL_VAR_STRING_8: ${MY_GLOBAL_VAR_STRING}")
message("MY_GLOBAL_VAR_GLOBAL_FUNC_2: ${MY_GLOBAL_VAR_GLOBAL_FUNC}")
 
message("\n###### END_Test_From_Cache_To_Normal")
 
message("\n###### BEGIN_Test_From_Normal_To_Cache\n")
 
# 1.3 修改同名缓存变量（普通变量晋升为全局变量示例）
set(MY_GLOBAL_VAR_STRING "abcdef modify after set normal variable" CACHE STRING "修改STRING缓存变量" FORCE)
message("MY_GLOBAL_VAR_STRING_9: ${MY_GLOBAL_VAR_STRING}\n")
 
test_global_var("111")
message("\nMY_GLOBAL_VAR_STRING_11: ${MY_GLOBAL_VAR_STRING}")
message("MY_GLOBAL_VAR_GLOBAL_FUNC_3: ${MY_GLOBAL_VAR_GLOBAL_FUNC}\n")
 
test_normal_var("222")
message("\nMY_GLOBAL_VAR_STRING_10: ${MY_GLOBAL_VAR_STRING}")
message("MY_LOCAL_VAR_NORMAL_FUNC_3: ${MY_LOCAL_VAR_NORMAL_FUNC}")
 
message("\n###### END_Test_From_Normal_To_Cache")
 
# 打印结束日志
message(STATUS "########## END_TEST_CACHE_VARIABLE\n")
```

### 5.2 运行结果
- 本地环境
  同上文示例中的环境，不做赘述。
- 输出结果
```
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.19044.
--
########## BEGIN_TEST_CACHE_VARIABLE
MY_GLOBAL_VAR_STRING_1: abcdef

###### BEGIN_Test_From_Cache_To_Normal

MY_GLOBAL_VAR_STRING_2: I am a normal variable but name same as cache variable

MY_GLOBAL_VAR_STRING_3: I am a normal variable but name same as cache variable
MY_GLOBAL_VAR_STRING_4: I am a normal variable but name same as cache variable
MY_LOCAL_VAR_NORMAL_FUNC_1: my local var define in normal func :: aaa

MY_GLOBAL_VAR_STRING_7: modify by in normal func inner :: aaa
MY_LOCAL_VAR_NORMAL_FUNC_2:

MY_GLOBAL_VAR_STRING_5: modify by in normal func inner :: aaa
MY_GLOBAL_VAR_STRING_6: modify by in global func inner :: bbb
MY_GLOBAL_VAR_GLOBAL_FUNC_1: my global var define in global func :: bbb

MY_GLOBAL_VAR_STRING_8: modify by in normal func inner :: aaa
MY_GLOBAL_VAR_GLOBAL_FUNC_2: my global var define in global func :: bbb

###### END_Test_From_Cache_To_Normal

###### BEGIN_Test_From_Normal_To_Cache

MY_GLOBAL_VAR_STRING_9: abcdef modify after set normal variable

MY_GLOBAL_VAR_STRING_5: abcdef modify after set normal variable
MY_GLOBAL_VAR_STRING_6: modify by in global func inner :: 111
MY_GLOBAL_VAR_GLOBAL_FUNC_1: my global var define in global func :: 111

MY_GLOBAL_VAR_STRING_11: modify by in global func inner :: 111
MY_GLOBAL_VAR_GLOBAL_FUNC_3: my global var define in global func :: 111

MY_GLOBAL_VAR_STRING_3: modify by in global func inner :: 111
MY_GLOBAL_VAR_STRING_4: modify by in global func inner :: 111
MY_LOCAL_VAR_NORMAL_FUNC_1: my local var define in normal func :: 222

MY_GLOBAL_VAR_STRING_10: modify by in normal func inner :: 222
MY_LOCAL_VAR_NORMAL_FUNC_3:

###### END_Test_From_Normal_To_Cache
-- ########## END_TEST_CACHE_VARIABLE

-- Configuring done
-- Generating done
-- Build files have been written to: E:/learning/learning-programming/modern-cmake/materials/缓存变量/cache3/build
```

### 5.3 小结
- 普通变量若与缓存变量同名（PS：注意顺序，先有缓存变量，后定义普通变量），缓存变量会变为普通变量。
  CMake 语法规定，当有一个与 Cache 变量同名的 Normal 变量出现时，后面使用这个变量都被当做 Normal 变量；如果一直没有同名的 Normal 变量，CMake 会自动认定这个变量一直为 Cache 变量。

  如示例中第19行，定义了一个Cache变量；第49行，又定义了一个同名的普通变量，从这里此变量“沦为”为了普通变量。

  为了验证，我们使用测试普通变量的函数进行了校核（第52行），的确与普通变量在函数作用域中应用表现一致。

  另外，注意示例程序第56行，当我们用测试缓存变量的函数进行校核时（示例程序第39、57行的运行结果在第30、33行），发现在函数作用域中将其修改为缓存变量后，只在函数作用域中有效。

  当退出函数作用域后，此变量依然是原旧值，即仍旧是普通变量。但是，与之形成鲜明对比的，在函数作用域中新定义的缓存变量第42行，从示例程序中第43、58行的运行结果31、34行分析，的确是全局有效的，因为值完全相同。

  重要推论：**父作用域的普通变量，在子作用域中即使修改为缓存变量，仅仅在子作用域中有效，退出子作用域后失效，即仍旧是普通变量。**

- 相反：缓存变量若与普通变量同名，普通变量会"晋升"为缓存变量。
  如示例中第65行，再定义了一个同名的缓存变量，从这里，此变量又“晋升”为了缓存变量。
  为了验证，我们使用测试缓存变量的函数进行了校核时（第68行），的确与缓存变量在函数作用域中应用表现一致。

  另外，注意示例程序第72行，当我们用测试普通变量的函数进行校核时（示例程序第27、73行的运行结果在第50、53行），发现在函数作用域中将其修改为普通变量后，当退出函数作用域后，此变量的值才改变为修改后的值。

  在函数内部之所以没有改变（如运行结果第50行），因为进入函数的瞬时，变量的值为运行结果第49行打印的值，而示例程序第26行修改时添加PARENT_SCOPE选项，明确修改的是父作用域的值，所以对当前函数作用域”无关痛痒“。

  最后，再明确一点：当72行调用函数test_normal_var结束后，该变量已变为普通变量，不再是缓存变量。

  重要推论：**父作用域的缓存变量，在子作用域中修改为普通变量（加PARENT_SCOPE选项），退出子作用域后仍旧是普通变量。**

- **建议尽量避免两者同名，容易混淆。**
  学习这个目的：一方面，满足好奇心，探究一下；二方面，为了假如遇到类似问题，排查分析时有个基本规则认知。但强烈建议不要这样使用，个人拙见，仅供参考。

## 6 通过终端预设缓存变量值
### 6.1 应用示例
- 代码结构 见./materials/缓存变量/cache4
- 示例程序（CMakeLists.txt）
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
message(STATUS "\n########## BEGIN_TEST_CACHE_VARIABLE")
 
message("${PROJECT_NAME}_GLOBAL_VAR_1: ${${PROJECT_NAME}_GLOBAL_VAR}")
 
# 修改缓存变量
set(${PROJECT_NAME}_GLOBAL_VAR "I am a cache variable" CACHE STRING "定义一个STRING缓存变量" FORCE)
message("${PROJECT_NAME}_GLOBAL_VAR_2: ${${PROJECT_NAME}_GLOBAL_VAR}")
 
# 打印结束日志
message(STATUS "########## END_TEST_CACHE_VARIABLE\n")
```

- 配置脚本（cmake_config.bat）
```
@echo off
set currentDir=%~dp0
set buildDir=%currentDir%
set cmakeOutputDir=%currentDir%\build
cmake -S %buildDir% -B %cmakeOutputDir% -G"Visual Studio 16 2019" -T v140 -A x64 -D KAIZEN_GLOBAL_VAR="abcdef"
pause
```

### 6.2 运行结果
- 本地环境
  同上文示例中的环境，不做赘述。

- 运行结果
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
########## BEGIN_TEST_CACHE_VARIABLE
KAIZEN_GLOBAL_VAR_1: abcdef
KAIZEN_GLOBAL_VAR_2: I am a cache variable
-- ########## END_TEST_CACHE_VARIABLE

-- Configuring done
-- Generating done
-- Build files have been written to: E:/learning/learning-programming/modern-cmake/materials/缓存变量/cache4/build
```

### 6.3 小结
- 在终端配置CMake时，通过 -D 变量名=变量值 方式可以设定默认存在的Cache变量 或 修改缓存变量值。

## 7 缓存变量应用注意事项
### 7.1 缓存变量，本质是全局变量
可以把缓存变量当做C、C++中的全局变量理解即可。类比法理解与体会，更易于学习与应用。

### 7.2 缓存变量，都会存储在CMakeCache.txt文件中
当你确认某个变量是缓存变量时，理论上你一定可以在CMakeCache.txt中找到此变量的记录项。

CMakeCache.txt文件中，还有很多默认的缓存变量，可自行查看与分析研究。

### 7.3 缓存变量发生问题，一定记得先删除build目录下的CMakeCache.txt文件，然后重新配置项目