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