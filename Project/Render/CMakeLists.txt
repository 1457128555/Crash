
#   GLOB_RECURSE 递归搜索所有文件
file(GLOB_RECURSE CRASH_RENDER_HEADER_FILES Include/*.h  Include/*.hpp)
file(GLOB_RECURSE CRASH_RENDER_SOURCE_FILES Source/*.cpp Source/*.c)

add_library(CrashRender SHARED 
    ${CRASH_RENDER_HEADER_FILES} 
    ${CRASH_RENDER_SOURCE_FILES}
)

target_include_directories(CrashRender PUBLIC Include)

target_link_libraries(CrashRender PUBLIC CrashCommon)

target_link_libraries(CrashRender PRIVATE glad)

#target_compile_definitions(CrashRender PRIVATE CRASH_RENDER_EXPORTS)