cmake_minimum_required(VERSION 3.10)

# 定义项目基准信息
set(CHAR_PROJ_NAME "MP-PMT")
project(${CHAR_PROJ_NAME} VERSION 1.0 LANGUAGES CXX)


# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁用编译器扩展，保持标准
set(CMAKE_CXX_EXTENSIONS OFF) 

# 添加可执行文件
add_executable(${CHAR_PROJ_NAME}
    src/main.cpp
    src/demo.cpp
)

# 为 Demo 指定 include 路径
target_include_directories(${CHAR_PROJ_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
# include_directories(include)  # 弃用高耦合方案