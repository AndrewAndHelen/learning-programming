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