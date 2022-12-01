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