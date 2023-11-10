cmake_minimum_required(VERSION 3.20)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "SDL2/cmake")

project(admirals)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set SDL2_DIR to the path of your SDL2 directory
set(SDL2_DIR "SDL2")

# Find Vulkan and SDL2
find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

# Add source files here.
set(SOURCES 
    src/Engine.cpp
)

# List all the VK2D source files
set(VK2D_SOURCES
    Vulkan2D/VK2D/Buffer.c
    Vulkan2D/VK2D/Camera.c
    Vulkan2D/VK2D/Constants.c
    Vulkan2D/VK2D/DescriptorBuffer.c
    Vulkan2D/VK2D/DescriptorControl.c
    Vulkan2D/VK2D/Image.c
    Vulkan2D/VK2D/Initializers.c
    Vulkan2D/VK2D/LogicalDevice.c
    Vulkan2D/VK2D/Model.c
    Vulkan2D/VK2D/PhysicalDevice.c
    Vulkan2D/VK2D/Pipeline.c
    Vulkan2D/VK2D/Polygon.c
    Vulkan2D/VK2D/Renderer.c
    Vulkan2D/VK2D/RendererMeta.c
    Vulkan2D/VK2D/Shader.c
    Vulkan2D/VK2D/Texture.c
    Vulkan2D/VK2D/Util.c
    Vulkan2D/VK2D/Validation.c
    Vulkan2D/VK2D/VulkanInterface.c
)

set_source_files_properties(${VK2D_SOURCES} PROPERTIES LANGUAGE C)

set(VMA_FILES Vulkan2D/VulkanMemoryAllocator/include/vk_mem_alloc.h Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp)

message(VK2D_C ${VK2D_C})

# Create a library with the sources
add_library(admirals ${SOURCES} ${VMA_FILES} ${VK2D_SOURCES})

# Replace "ExternalLibrary" with the actual name of the external library

target_include_directories(admirals PRIVATE "Vulkan2D")
target_include_directories(admirals PRIVATE ${SDL2_INCLUDE_DIRS} ${Vulkan_INCLUDE_DIRS} "src/" "Vulkan2D/VulkanMemoryAllocator/src")
target_link_libraries(admirals PRIVATE ${SDL2_LIBRARIES} ${Vulkan_LIBRARY})

## Create test executable
add_executable(demo_render demo/render/debug.cpp demo/render/main.cpp ${VK2D_SOURCES})
target_link_libraries(demo_render PRIVATE admirals)
target_include_directories(demo_render PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
    "${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D"
    "Vulkan2D/VulkanMemoryAllocator/src"
    "demo/render"
)
