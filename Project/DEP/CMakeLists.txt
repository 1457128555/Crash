add_subdirectory(GLM)
add_subdirectory(GLAD)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libs for assimp" FORCE)

# Remove debug postfix for assimp
set(ASSIMP_DEBUG_POSTFIX "" CACHE STRING "Remove debug postfix for assimp" FORCE)
set(CMAKE_DEBUG_POSTFIX "" CACHE STRING "Remove debug postfix globally" FORCE)

add_subdirectory(assimp)
unset(BUILD_SHARED_LIBS CACHE)

if(WIN32)
    add_subdirectory(GLFW)
    add_subdirectory(IMGUI)
endif()

