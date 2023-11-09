cmake_minimum_required(VERSION 3.20)
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
list(APPEND CMAKE_PREFIX_PATH "SDL2/cmake")

project(admirals CXX)

set(CMAKE_CXX_STANDARD 20)

# Set SDL2_DIR to the path of your SDL2 directory
set(SDL2_DIR "SDL2")



set(Vulkan_LIBRARY "${VULKAN_SDK}/Lib/vulkan-1.lib")
set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK}/Include")

# Find Vulkan and SDL2
find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

message("SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIR}")
message("SDL2_LIBRARIES: ${SDL2_LIBRARIES}")

set(SOURCES 
    src/Engine.cpp
)

set(VMA_FILES Vulkan2D/VulkanMemoryAllocator/include/vk_mem_alloc.h Vulkan2D/VulkanMemoryAllocator/src/VmaUsage.cpp)

# Set the source files for the main directory
file(GLOB VK2D_C Vulkan2D/VK2D/*.c)
file(GLOB VK2D_H Vulkan2D/VK2D/*.h)

# Create a library with the sources
add_library(admirals ${SOURCES} ${VMA_FILES} ${VK2D_C} ${VK2D_H})

# Replace "ExternalLibrary" with the actual name of the external library

target_include_directories(admirals PRIVATE "Vulkan2D")

target_include_directories(admirals PRIVATE "${CMAKE_SOURCE_DIR}/SDL2/include" ${Vulkan_INCLUDE_DIRS} "src/" "Vulkan2D/VulkanMemoryAllocator/src")

target_link_libraries(admirals PRIVATE ${SDL2_LIBRARIES} ${Vulkan_LIBRARIES})
