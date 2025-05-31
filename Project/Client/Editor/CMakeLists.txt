
file(GLOB_RECURSE CRASH_EDITOR_HEADER_FILES Include/*.h  Include/*.hpp)
file(GLOB_RECURSE CRASH_EDITOR_SOURCE_FILES Source/*.cpp Source/*.c)

add_executable(CrashEditor
    ${CRASH_CLIENT_HEADER_FILES} 
    ${CRASH_CLIENT_SOURCE_FILES}
    ${CRASH_EDITOR_HEADER_FILES} 
    ${CRASH_EDITOR_SOURCE_FILES}
)

target_include_directories(CrashEditor PUBLIC Include)
target_include_directories(CrashEditor PUBLIC ../Include)

target_link_libraries(CrashEditor PUBLIC
    glfw
    CrashEngine 
    CrashCommon
)

target_link_libraries(CrashEditor PRIVATE ImGui)

# 获取动态库的路径
add_custom_command(TARGET CrashEditor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:CrashEngine>      # CrashEngine.dll
        $<TARGET_FILE:CrashCommon>      # CrashCommon.dll
        $<TARGET_FILE:CrashRender>      # CrashRender.dll
        $<TARGET_FILE_DIR:CrashEditor>  # 复制到 CrashEditor 的输出目录
)

target_compile_definitions(CrashEditor PRIVATE CRASH_EDITOR)