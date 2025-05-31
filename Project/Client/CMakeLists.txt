file(GLOB_RECURSE CRASH_CLIENT_HEADER_FILES Include/*.h  Include/*.hpp)
file(GLOB_RECURSE CRASH_CLIENT_SOURCE_FILES Source/*.cpp Source/*.c)

add_subdirectory(Editor)
add_subdirectory(Runtime/Windows)