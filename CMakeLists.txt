cmake_minimum_required(VERSION 3.20)
list(APPEND CMAKE_PREFIX_PATH "include/SDL2/cmake")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

project(admirals)

# Set SDL2_DIR to the path of your SDL2 directory
set(SDL2_DIR include/SDL2)
set(VK2D_DIR include/Vulkan2D)
set(ASIO_DIR include/asio/asio/include)

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
message("CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")

set(SOURCES 
    src/engine/DataObjects.cpp
    src/engine/Engine.cpp
    src/engine/IDisplayLayer.cpp
    src/engine/PathFinding.cpp
    src/engine/QuadTree.cpp
    src/engine/Renderer.cpp
    src/engine/Scene.cpp
    src/engine/UI/Button.cpp
    src/engine/UI/Data.cpp
    src/engine/UI/DisplayLayout.cpp
    src/engine/UI/Element.cpp
    src/engine/UI/TextElement.cpp
    src/engine/UI/menu/Menu.cpp
    src/engine/UI/menu/MenuOption.cpp
    src/engine/net/Client.cpp
    src/engine/net/Connection.cpp
    src/engine/net/Server.cpp
)
set(MVP_SOURCES 
    src/mvp/MvpServer.cpp
    src/mvp/managers/GameManager.cpp
    src/mvp/managers/Animator.cpp
    src/mvp/managers/MenuManager.cpp
    src/mvp/managers/NetworkManager.cpp
    src/mvp/managers/SelectionManager.cpp
    src/mvp/objects/Base.cpp
    src/mvp/objects/Background.cpp
    src/mvp/objects/Grid.cpp
    src/mvp/objects/GridObject.cpp
    src/mvp/objects/Quad.cpp
    src/mvp/objects/Ship.cpp
    src/mvp/objects/Sprite.cpp
    src/mvp/objects/TreasureIsland.cpp
    src/mvp/objects/Decoration.cpp
)

set(INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/
    ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/net
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

add_library(asio INTERFACE)
target_include_directories(asio 
    INTERFACE
    ${ASIO_DIR}
)

# Create a library with the sources
add_library(engine SHARED ${SOURCES})
if(WIN32)
    target_link_libraries(engine wsock32)
    target_link_libraries(engine ws2_32)
endif()
target_link_libraries(engine vk2d asio)
target_include_directories(engine PUBLIC ${INCLUDE_DIRS})

# Create mvp executable
add_executable(mvp src/mvp/main.cpp ${MVP_SOURCES})
target_link_libraries(mvp engine)
target_include_directories(mvp PRIVATE
    "src/mvp/"
)

# Create mvp server
add_executable(mvp_server src/mvp/DedicatedServer.cpp src/mvp/MvpServer.cpp)
target_link_libraries(mvp_server engine)
target_include_directories(mvp_server PRIVATE
    "src/mvp/"
)

# Create test executable
add_executable(ui_test src/test/ui.test.cpp)
target_link_libraries(ui_test engine)
target_include_directories(ui_test PRIVATE 
    "src/test/"
)

# Create test executable
add_executable(gameObject_test src/test/gameObject.test.cpp)
target_link_libraries(gameObject_test engine)
target_include_directories(gameObject_test PRIVATE 
    "src/test/"
)
    
# Test client
add_executable(test_client src/test/testclient.cpp)
target_link_libraries(test_client engine)
target_include_directories(test_client PRIVATE 
    "src/test/"
)

# Test server
add_executable(test_server src/test/testserver.cpp)
target_link_libraries(test_server engine)
target_include_directories(test_server PRIVATE 
    "src/test/"
)

# Create test executable
add_executable(depthOrderedCollection_test src/test/depthOrderedCollection.test.cpp)
target_link_libraries(depthOrderedCollection_test engine cppunit)
target_include_directories(depthOrderedCollection_test PRIVATE 
    "src/test/"
)

# Create test executable
add_executable(vector_test src/test/vector.test.cpp)
target_link_libraries(vector_test engine cppunit)
target_include_directories(vector_test PRIVATE 
    "src/test/"
)

# Copy SDL2.dll to the output directory after the executable is built
add_custom_command(TARGET ui_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_DLL}"
        $<TARGET_FILE_DIR:ui_test>
)
