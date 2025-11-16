cmake_minimum_required(VERSION 3.10)

# 定义项目基准信息
set(CHAR_PROJ_NAME "MP-PMT")
project(${CHAR_PROJ_NAME} VERSION 1.0 LANGUAGES CXX)

# Compiler whitelist check
string(TOLOWER "${CMAKE_CXX_COMPILER_ID}" COMPILER_ID_LOWER)
set(ALLOWED_COMPILERS "gnu" "clang")
list(FIND ALLOWED_COMPILERS "${COMPILER_ID_LOWER}" COMPILER_FOUND)
if (COMPILER_FOUND EQUAL -1)
    string(JOIN ", " ALLOWED_COMPILERS_STR ${ALLOWED_COMPILERS})
    message(FATAL_ERROR
        "[Compiler Check Failed] This project can only be built with GCC or Clang compilers.\n"
        "   - Detected compiler ID: ${CMAKE_CXX_COMPILER_ID}\n"
        "   - Allowed compiler IDs: ${ALLOWED_COMPILERS_STR}\n"
        "   - Please ensure your environment variables or CMake generator selection is correct."
    )
endif()

message(STATUS "[Compiler Check Passed] Building the project with ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION}).")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 禁用编译器扩展，保持标准
set(CMAKE_CXX_EXTENSIONS OFF) 

# 启用优化
add_compile_options(-O2)

# 依赖库
#   1. OpenSSL  3.6.0 1
#   1. XSIMD    13.2.0
find_package(OpenSSL REQUIRED)          # OpenSSL
find_package(xsimd CONFIG REQUIRED)     # XSIMD

# 添加可执行文件
add_executable(${CHAR_PROJ_NAME} 
    src/main.cpp
    src/core/crc/crc64_ecma182.cpp
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

# 链接依赖
target_link_libraries(${CHAR_PROJ_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(${CHAR_PROJ_NAME}  PRIVATE xsimd)


# 指定 include 路径
target_include_directories(${CHAR_PROJ_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
# include_directories(include)  # 弃用高耦合方案