
#   GLOB_RECURSE 递归搜索所有文件
file(GLOB_RECURSE CRASH_COMMON_HEADER_FILES Include/*.h Include/*.hpp)
file(GLOB_RECURSE CRASH_COMMON_SOURCE_FILES Source/*.cpp Source/*.c)

add_library(CrashCommon SHARED 
    ${CRASH_COMMON_HEADER_FILES} 
    ${CRASH_COMMON_SOURCE_FILES}
)

target_link_libraries(CrashCommon PUBLIC glm)

target_include_directories(CrashCommon PUBLIC Include)

#target_compile_definitions(CrashCommon PRIVATE CRASH_COMMON_EXPORTS)

# 添加预编译头文件
target_precompile_headers(CrashCommon PUBLIC Include/CrashPCH.h)