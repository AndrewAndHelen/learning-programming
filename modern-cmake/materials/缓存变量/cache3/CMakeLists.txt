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