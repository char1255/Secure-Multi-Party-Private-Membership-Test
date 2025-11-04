cmake_minimum_required(VERSION 3.10)

# 定义项目基准信息
set(CHAR_PROJ_NAME "MP-PMT")
project(${CHAR_PROJ_NAME} VERSION 1.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁用编译器扩展，保持标准
set(CMAKE_CXX_EXTENSIONS OFF) 

# 依赖库
# 查找并链接 OpenSSL
find_package(OpenSSL REQUIRED)

# 添加可执行文件
add_executable(${CHAR_PROJ_NAME} 
    src/main.cpp
    src/core/ring/rvector_stl.cpp
    src/core/ring/rvector_xsimd.cpp
    src/core/protocol/ass_impl/agent_ass.cpp
    src/core/protocol/ass_impl/data_holder_ass.cpp
    src/core/protocol/ass_impl/querier_ass.cpp
    src/core/protocol/fss_impl/agent_fss.cpp
    src/core/protocol/fss_impl/data_holder_fss.cpp
    src/core/protocol/fss_impl/querier_fss.cpp
    src/auxkit/profiler.cpp
)

# src/core/ring/rvector_cuda.cu

# 将 OpenSSL 链接到你的目标
target_link_libraries(${CHAR_PROJ_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# 指定 include 路径
target_include_directories(${CHAR_PROJ_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
# include_directories(include)  # 弃用高耦合方案