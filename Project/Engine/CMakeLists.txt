
#   GLOB_RECURSE 递归搜索所有文件
file(GLOB_RECURSE CRASH_ENGINE_HEADER_FILES Include/*.h Include/*.hpp)
file(GLOB_RECURSE CRASH_ENGINE_SOURCE_FILES Source/*.cpp Source/*.c)

add_library(CrashEngine SHARED 
    ${CRASH_ENGINE_HEADER_FILES} 
    ${CRASH_ENGINE_SOURCE_FILES}
)

target_include_directories(CrashEngine PUBLIC Include)

target_link_libraries(CrashEngine PUBLIC  
    CrashCommon 
    CrashRender
)

target_link_libraries(CrashEngine PRIVATE assimp)

#target_compile_definitions(CrashEngine PRIVATE CRASH_ENGINE_EXPORTS)