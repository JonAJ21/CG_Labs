cmake_minimum_required(VERSION 3.10)

project(CG_Labs)

# Указываем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Устанавливаем политику для OpenGL
cmake_policy(SET CMP0072 NEW)

# Добавляем исполняемый файл
add_executable(CG_Labs
    src/main.cpp
    src/SFML/window.cpp
    src/GL/vao.cpp
    src/GL/program.cpp
    src/Engine/Scene/scene.cpp
    src/Engine/Camera/camera.cpp
    src/Engine/Control/controller.cpp
    src/Engine/Geometry/2D/polygon.cpp
    src/Engine/Geometry/3D/cylinder.cpp
    src/Engine/Geometry/3D/sphere.cpp
    src/Engine/Geometry/3D/pyramid.cpp
    src/Engine/Light/spotlight.cpp
)

# Указываем пути к заголовочным файлам
include_directories(include)


# Находим пакеты
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

# Путь к glm
find_path(GLM_INCLUDE_DIR glm/glm.hpp)
if (GLM_INCLUDE_DIR)
    include_directories(${GLM_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Could not find glm")
endif()

# Линкуем библиотеки
target_link_libraries(CG_Labs 
    OpenGL::GL 
    GLEW::GLEW 
    sfml-system 
    sfml-window 
    sfml-graphics
)