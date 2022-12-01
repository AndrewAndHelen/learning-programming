# Cmake Tutorial

- （零）前言
- （一）CMAKE变量分类及其含义
- （二）CMAKE运算符、流程控制与基本命令
- （三）CMAKE由浅入深实战训练

## （零）前言
### 1. C++程序编译过程
```
源程序经过预处理、编译、汇编、链接步骤后，才能生成可执行程序。

预处理：条件编译，头文件包含，宏替换的处理，刪除注释，生成.i文件。
gcc -E hello_world.c -o hello_world.i

编译：将预处理后的文件转换成汇编语言，生成.s文件
gcc -S

汇编：汇编变为目标代码(机器代码)生成.o的文件
gcc -c hello_world.s -o hello_world.o

链接：连接目标代码,生成可执行程序
gcc hello.world.o -o hello_world
```
### 2. CMake的作用
```
CMake是一种跨平台编译工具，主要是编写CMakeLists.txt文件，用cmake命令将CMakeLists.txt文件转化为make所需要的makefile文件，
最后用make命令编译源码生成可执行程序或共享库(shared object)。特点：开源、跨平台、可扩展、简化编译构建过程和编译过程。
```

## （一）CMAKE变量分类及其含义
- 1 CMake变量分类
    - 1.1 普通变量
    - 1.2 缓存变量
    - 1.3 环境变量
- 2 CMake内置变量分类
    - 2.1 提供信息的变量
    - 2.2 改变行为的变量
    - 2.3 描述系统的变量
    - 2.4 控制构建的变量
    - 2.5 语言变量
    - 2.6 CTest变量
    - 2.7 CPack变量
    - 2.8 内部变量（不推荐再使用）
    - 2.9 常用的内置变量

### 1 CMake变量分类
#### 1.1 普通变量
 [普通变量与函数](./materials/普通变量与函数/ReadMe.md)
 [普通变量与子目录](./materials/普通变量与子目录/ReadMe.md)
 [普通变量与包含下_宏和函数](./materials/普通变量与包含下_宏和函数/ReadMe.md)
#### 1.2 缓存变量
 [缓存变量](./materials/缓存变量/ReadMe.md)
#### 1.3 环境变量
 [环境变量](./materials/环境变量/ReadMe.md)
### 2 CMake内置变量分类
#### 2.1 提供信息的变量
​ 详情可参见官方文档[Variables that Provide Information](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-that-provide-information)

#### 2.2 改变行为的变量
​ 详情可参见官方文档[Variables that Change Behavior](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-that-change-behavior)

#### 2.3 描述系统的变量
​ 详情可参见官方文档[Variables that Describe the System](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-that-describe-the-system)

#### 2.4 控制构建的变量
​ 详情可参见官方文档[Variables that Control the Build](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-that-control-the-build)

#### 2.5 语言变量
​ 详情可参见官方文档[Variables for Languages](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-for-languages)

#### 2.6 CTest变量
​ 详情可参见官方文档[Variables for CTest](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-for-ctest)

#### 2.7 CPack变量
​ 详情可参见官方文档[Variables for CPack](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#variables-for-cpack)

#### 2.8 内部变量（不推荐再使用）
  ​CMake有很多内部变量。他们中的大多数都无正式文档记录。
​  然而，其中一些变量在某些时候被描述为普通变量，因此可能会在遗留代码中遇到。
​  它们意思可能会发生变化，不建议在项目代码中使用。
​  详情可参见官方文档[Internal Variables](https://cmake.org/cmake/help/latest/manual/cmake-variables.7.html?highlight=variable#internal-variables)

#### 2.9 常用的内置变量
<table style="text-align:center;">
<tr>
  <th align="center">变量名</th>
  <th align="center">含义</th>
</tr>
<tr>
  <td colspan="2"><b>1.提供信息的变量</b></td>
</tr>
<tr>
  <td>CMAKE_PROJECT_NAME</td>
  <td>当前工程名字</td>
</tr>
<tr>
  <td>PROJECT_SOURCE_DIR</td>
  <td>当前工程的根目录</td>
</tr>
<tr>
  <td>CMAKE_SOURCE_DIR</td>
  <td>顶层CMakeLists.txt所在的路径</td>
</tr>
<tr>
  <td>CMAKE_CURRENT_SOURCE_DIR</td>
  <td>这是当前处理的CMakeLists.txt所在的目录</td>
</tr>
<tr>
  <td>CMAKE_CURRENT_BINARY_DIR</td>
  <td>target 编译目录，每个子project都会有一个编译目录</td>
</tr>
<tr>
  <td>CMAKE_CURRENT_LIST_DIR</td>
  <td>当前处理的'cmake文件'所在的目录</td>
</tr>
<tr>
  <td>CMAKE_CURRENT_LIST_FILE</td>
  <td>当前处理的'CMakeLists.txt或cmake'文件的全路径</td>
</tr>
<tr>
  <td>CMAKE_CURRENT_LIST_LINE</td>
  <td>当前所在的行</td>
</tr>
<tr>
  <td colspan="2"><b>2.改变行为的变量</b></td>
</tr>
<tr>
  <td>CMAKE_INSTALL_PREFIX</td>
  <td> 使用的安装目录</td>
</tr>
<tr>
  <td>CMAKE_MODULE_PATH</td>
  <td> cmake 模块所在的目录</td>
</tr>
<tr>
  <td>CMAKE_PREFIX_PATH</td>
  <td> 以分号分隔的目录列表，find_XXX会添加其相应的子目录（如bin、lib或include），默认为空</td>
</tr>
<tr>
  <td>&lt;PackageName&gt;_ROOT</td>
  <td> find_package(&lt;PackageName&gt)将搜索&lt;PackageName&gt_ROOT变量指定的前缀</td>
</tr>
<tr>
  <td colspan="2"><b>3.描述系统的变量</b></td>
</tr>
<tr>
  <td>­CMAKE_MAJOR_VERSION</td>
  <td>cmake 主版本号，比如 3.4.1 中的 3</td>
</tr>
<tr>
  <td>CMAKE_MINOR_VERSION</td>
  <td>cmake 次版本号，比如 3.4.1 中的 4</td>
</tr>
<tr>
  <td>CMAKE_PATCH_VERSION</td>
  <td>cmake 补丁等级，比如 3.4.1 中的 1</td>
</tr>
<tr>
  <td>CMAKE_SYSTEM</td>
  <td>系统名称，比如 Linux-­2.6.22</td>
</tr>
<tr>
  <td>CMAKE_SYSTEM_NAME</td>
  <td>不包含版本的系统名，比如 Linux</td>
</tr>
<tr>
  <td>CMAKE_SYSTEM_VERSION</td>
  <td>系统版本，比如 2.6.22</td>
</tr>
<tr>
  <td>CMAKE_SYSTEM_PROCESSOR</td>
  <td>处理器名称，比如 i686</td>
</tr>
<tr>
  <td>UNIX</td>
  <td> 在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin</td>
</tr>
<tr>
  <td>WIN32</td>
  <td> 在所有的 win32 平台下该值为 TRUE，包括 cygwin</td>
</tr>
<tr>
  <td colspan="2"><b>4.控制构建的变量</b></td>
</tr>
<tr>
  <td>CMAKE_INCLUDE_CURRENT_DIR</td>
  <td> 如果启用此变量，CMake 会自动添加 CMAKE_CURRENT_SOURCE_DIR和CMAKE_CURRENT_BINARY_DIR 到每个目录的包含路径，默认情况下CMAKE_INCLUDE_CURRENT_DIR是OFF</td>
</tr>
<tr>
  <td>CMAKE_LIBRARY_OUTPUT_DIRECTORY</td>
  <td>构建时将所有LIBRARY 目标文件放在哪里的目录</td>
</tr>
<tr>
  <td colspan="2"><b>5.语言变量</b></td>
</tr>
<tr>
  <td>CMAKE_C_STANDARD</td>
  <td>C语言标准支持级别</td>
</tr>
<tr>
  <td>CMAKE_CXX_STANDARD</td>
  <td>C++语言标准支持级别</td>
</tr>
<tr>
  <td>CMAKE_COMPILER_IS_GNUCC</td>
  <td>编译期是否为GCC</td>
</tr>
<tr>
  <td colspan="2"><b>6.CTest变量</b></td>
</tr>
<tr>
  <td colspan="2"><b>7.CPack变量</b></td>
</tr>
</table>

## （二）CMAKE运算符、流程控制与基本命令
- 1.运算符
  - 1.1 一元运算符
  - 1.2 二元运算符
  - 1.3 与或非运算符
- 2.流程控制
- 3.基本命令
### 1.运算符 
#### 1.2 一元运算符
#### 1.3 二元运算符
#### 1.4 逻辑运算符
[CMAKE运算符](./materials/运算符/ReadMe.md)

### 2.流程控制
[CMAKE流程控制](./materials/流程控制/ReadMe.md)

### 3.基本命令
[CMAKE基本命令](./materials/基本命令/ReadMe.md)


## 参考与引用
1. [（一）CMAKE变量分类及其含义](https://www.bbsmax.com/A/gGdXeaeYz4/#21-%E6%8F%90%E4%BE%9B%E4%BF%A1%E6%81%AF%E7%9A%84%E5%8F%98%E9%87%8F)
2. [CMakeLists详解](https://blog.csdn.net/weixin_43837968/article/details/115257575?ops_request_misc=&request_id=&biz_id=102&utm_term=CMAKE_CURRENT_LIST_DIR&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-3-115257575.142^v65^js_top,201^v3^control_1,213^v2^t3_esquery_v1&spm=1018.2226.3001.4187)
3. [CMake基本语法](https://blog.csdn.net/dadan1314/article/details/91959197)
4. [CMake编程实践（四）　控制指令](https://blog.csdn.net/MakerCloud/article/details/105759548)
5. [http://t.zoukankan.com/Long-w-p-9587808.html](http://t.zoukankan.com/Long-w-p-9587808.html)








