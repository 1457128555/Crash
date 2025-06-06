# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

cmake_minimum_required(VERSION 3.22.1)

project("runtime")

# Creates your game shared library. The name must be the same as the
# one used for loading in your Kotlin/Java or AndroidManifest.txt files.

set(RootPath ../../../../../../../)

add_subdirectory(${RootPath}/DEP        ${CMAKE_BINARY_DIR}/DEP)
add_subdirectory(${RootPath}/Common     ${CMAKE_BINARY_DIR}/Common)
add_subdirectory(${RootPath}/Render     ${CMAKE_BINARY_DIR}/Render)
add_subdirectory(${RootPath}/Engine     ${CMAKE_BINARY_DIR}/Engine)

file(GLOB_RECURSE CRASH_CLIENT_HEADER_FILES ${RootPath}/Client/Include/*.h  ${RootPath}/Client/Include/*.hpp)
file(GLOB_RECURSE CRASH_CLIENT_SOURCE_FILES ${RootPath}/Client/Source/*.cpp ${RootPath}/Client/Source/*.c)

add_library(runtime SHARED
        ${CRASH_CLIENT_HEADER_FILES}
        ${CRASH_CLIENT_SOURCE_FILES}
        main.cpp)

target_include_directories(runtime PUBLIC ${RootPath}/Client/Include)

# Searches for a package provided by the game activity dependency
find_package(game-activity REQUIRED CONFIG)



# Configure libraries CMake uses to link your target library.
target_link_libraries(runtime
        # The game activity
        game-activity::game-activity

        # EGL and other dependent libraries required for drawing
        # and interacting with Android system
        EGL
        GLESv3
        jnigraphics
        android
        log
        CrashEngine
        CrashCommon
)