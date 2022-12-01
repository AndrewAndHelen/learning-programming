cmake_minimum_required(3.18 REQUIRED)

# 设置工程名称
set(PROJECT_NAME OPERATOR)
 
# 设置工程版本号
set(PROJECT_VERSION "1.0.0.10" CACHE STRING "默认版本号")

# 工程定义
project(${PROJECT_NAME}
    LANGUAGES CXX C
    VERSION ${PROJECT_VERSION}
)

# 开始测试
message(STATUS "\n开始测试")

message(STATUS "\n开始测试一元操作符")


message(STATUS "\n开始测试二元操作符")


message(STATUS "\n开始测试逻辑操作符")

# 结束测试
message(STATUS "\n结束测试") 
