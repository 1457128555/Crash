

add_executable(CrashRuntime 
    ${CRASH_CLIENT_HEADER_FILES} 
    ${CRASH_CLIENT_SOURCE_FILES}
    main.cpp
)

target_include_directories(CrashRuntime PUBLIC ../../Include)

target_link_libraries(CrashRuntime PUBLIC
    glfw
    CrashEngine 
    CrashCommon
)

# 获取动态库的路径
add_custom_command(TARGET CrashRuntime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:CrashEngine>      # CrashEngine.dll
        $<TARGET_FILE:CrashCommon>      # CrashCommon.dll
        $<TARGET_FILE:CrashRender>      # CrashRender.dll
        $<TARGET_FILE_DIR:CrashRuntime>  # 复制到 CrashRuntime 的输出目录
)
