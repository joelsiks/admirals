cmake_minimum_required(VERSION 3.20)
list(APPEND CMAKE_PREFIX_PATH "include/SDL2/cmake")
project(admirals)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# Set SDL2_DIR to the path of your SDL2 directory
set(SDL2_DIR include/SDL2)
set(VK2D_DIR include/Vulkan2D)

# Find Vulkan and SDL2
find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

# Insert SDL2.dll location to SDL2_DLL environment variable.
get_target_property(SDL2_DLL SDL2::SDL2 IMPORTED_LOCATION)


message("SDL2.dll path: ${SDL2_DLL}")
message("SDL2_DIR: ${SDL2_DIR}")
message("SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIR}")
message("SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message("Vulkan_INCLUDE_DIR: ${Vulkan_INCLUDE_DIR}")
message("Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")

set(SOURCES 
    src/engine/Model.cpp
    src/engine/GameObject.cpp
    src/engine/Scene.cpp
    src/engine/Renderer.cpp
)

set(VMA_FILES ${VK2D_DIR}/VulkanMemoryAllocator/src/VmaUsage.cpp)

# Set the source files for the main directory
file(GLOB VK2D_C ${VK2D_DIR}/VK2D/*.c)
file(GLOB VK2D_H ${VK2D_DIR}/VK2D/*.h)
file(GLOB ASSETS src/assets/*)

message("COPYING ASSETS: ${ASSETS}")
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR}/assets)

# -----------------------------------------------------------------------------

add_library(vk2d SHARED ${VMA_FILES} ${VK2D_C} ${VK2D_H})
target_link_libraries(vk2d m ${SDL2_LIBRARIES} ${Vulkan_LIBRARY})
target_include_directories(vk2d 
    PUBLIC 
    ${SDL2_INCLUDE_DIRS}
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/${VK2D_DIR}
)

# Create a library with the sources
add_library(engine SHARED ${SOURCES})
target_link_libraries(engine vk2d)
target_include_directories(engine PRIVATE 
    "src/engine/" 
)

# Create test executable
add_executable(renderer_test src/test/renderer.test.cpp)
target_link_libraries(renderer_test vk2d)
target_include_directories(renderer_test PRIVATE 
    "src/test/"
)

# Create test executable
add_executable(gameObject_test src/test/gameObject.test.cpp)
target_link_libraries(gameObject_test engine)
target_include_directories(gameObject_test PRIVATE 
    "src/test/"
    "src/engine/"
)

# Copy SDL2.dll to the output directory after the executable is built
add_custom_command(TARGET renderer_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:renderer_test>
)
