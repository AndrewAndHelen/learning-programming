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