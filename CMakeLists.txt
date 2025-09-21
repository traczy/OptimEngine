cmake_minimum_required(VERSION 3.10)
project(OptimumEngineProject LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_PREFIX_PATH "C:/Users/jrbri/glfw-3.4/lib-mingw-w64/;${CMAKE_PREFIX_PATH}")

set(MY_SOURCES
    src/main.cpp
    src/Camera/Camera.cpp
    src/Camera/CameraController.cpp
    src/Lighting/DirectionalLight.cpp
    src/Lighting/DirectionalLightingController.cpp
    src/Lighting/PointLight.cpp
    src/Material/Material.cpp
    src/RenderObjects/Object.cpp
    src/Shaders/Shader.cpp
    src/Utility/HandleError.cpp
    src/Utility/Transform.cpp
    src/Utility/Logger.cpp
    src/Windowing/MainWindow.cpp
    src/Glad/glad.c
    include/Camera/Camera.h
    include/Camera/CameraController.h
    include/Lighting/DirectionalLight.h
    include/Lighting/DirectionalLightingController.h
    include/Lighting/PointLight.h
    include/Material/Material.h
    include/RenderObjects/Object.h
    include/Shaders/FragmentShader.h
    include/Shaders/Shader.h
    include/Shaders/VertexShader.h
    include/Utility/Constants.h
    include/Utility/HandleError.h
    include/Utility/Transform.h
    include/Utility/Logger.h
    include/Windowing/MainWindow.h
    include/Glad/glad.h
    include/Glad/KHR/khrplatform.h
)

add_executable(OptimumEngine ${MY_SOURCES})

target_include_directories(OptimumEngine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/Glad/
    $ENV{USERPROFILE}/glfw-3.4/include/
    $ENV{USERPROFILE}/glm-master/
    $ENV{USERPROFILE}/stb-master/
)

target_link_libraries(OptimumEngine PRIVATE "$ENV{USERPROFILE}/glfw-3.4/lib-vc2019/glfw3dll.lib")
target_link_libraries(OptimumEngine PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/OpenGL32.Lib")
target_link_libraries(OptimumEngine PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/kernel32.Lib")
target_link_libraries(OptimumEngine PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/User32.Lib")
target_link_libraries(OptimumEngine PRIVATE "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64/Gdi32.Lib")
