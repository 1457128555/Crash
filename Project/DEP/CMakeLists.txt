add_subdirectory(GLM)
add_subdirectory(GLAD)

if(WIN32)
    add_subdirectory(GLFW)
    add_subdirectory(IMGUI)
endif()

