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