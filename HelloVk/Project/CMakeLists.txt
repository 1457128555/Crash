
file(GLOB PROJECT_INC_FILES "Include/*.h" "Include/*.hpp")
file(GLOB PROJECT_SRC_FILES "Source/*.cpp" "Source/*.c")

add_executable(HelloVk ${PROJECT_INC_FILES} ${PROJECT_SRC_FILES})

target_include_directories(HelloVk PUBLIC Include)

find_package(Vulkan REQUIRED)
target_include_directories(HelloVk PRIVATE ${Vulkan_INCLUDE_DIRS})

target_link_libraries(HelloVk PUBLIC 
    ${Vulkan_LIBRARIES}
    glfw
    glm
)