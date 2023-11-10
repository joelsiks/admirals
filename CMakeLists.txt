cmake_minimum_required(VERSION 3.20)
list(APPEND CMAKE_PREFIX_PATH "SDL2/cmake")
project(admirals CXX)

set(CMAKE_CXX_STANDARD 20)

# Set SDL2_DIR to the path of your SDL2 directory
set(SDL2_DIR SDL2)

#set(Vulkan_LIBRARYS "${VULKAN_SDK}/Lib/vulkan-1.lib")
#set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK}/Include")


# Find Vulkan and SDL2
find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

message("SDL2_DIR: ${SDL2_DIR}")
message("SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIR}")
message("SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message("Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
message("Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")

set(SOURCES 
    src/Engine.cpp
)

set(VMA_FILES Vulkan2D/VulkanMemoryAllocator/include/vk_mem_alloc.h Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp)

# Set the source files for the main directory
file(GLOB VK2D_C Vulkan2D/VK2D/*.c)
file(GLOB VK2D_H Vulkan2D/VK2D/*.h)

# Create vk2d library
add_library(vk2d SHARED ${VK2D_C} ${VK2D_H} ${VMA_FILES})
target_link_libraries(vk2d ${SDL2_LIBRARIES} ${Vulkan_LIBRARY})
target_include_directories(vk2d 
    PUBLIC 
    ${SDL2_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/VK2D/
    ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/VulkanMemoryAllocator/src/
    ${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2D/VulkanMemoryAllocator/include/
)

# Create a library with the sources
add_library(admirals ${SOURCES})
target_link_libraries(admirals PRIVATE vk2d ${SDL2_LIBRARIES} ${Vulkan_LIBRARY})
target_include_directories(admirals PRIVATE 
    "src/" 
)

## Create test executable
add_executable(renderer_test src/test/renderer.test.cpp)
target_link_libraries(renderer_test PRIVATE vk2d ${SDL2_LIBRARIES} ${Vulkan_LIBRARY})
target_include_directories(renderer_test PRIVATE 
    "src/test/"
    "src/assets/"
)
