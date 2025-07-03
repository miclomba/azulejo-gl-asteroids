cmake_minimum_required(VERSION "3.16") 

project(GLAsteroids VERSION 1.0.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
# Require the C++17 or new compiler
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find Qt
find_package(Qt6 REQUIRED COMPONENTS QuickControls2 Widgets OpenGL OpenGLWidgets QmlImportScanner)
qt_standard_project_setup()
# Enable auto meta object compiler and the resource compiler (for QML support)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

message(STATUS "VCPKG_INCL_DIR: $ENV{VCPKG_INCL_DIR}")
message(STATUS "VCPKG_LINK_DIR: $ENV{VCPKG_LINK_DIR}")
message(STATUS "X_INCL_DIR: $ENV{X_INCL_DIR}")
message(STATUS "X_LINK_DIR: $ENV{X_LINK_DIR}")

include_directories(
"${CMAKE_SOURCE_DIR}/include"
"$ENV{X_INCL_DIR}"
"$ENV{VCPKG_INCL_DIR}"
)

link_directories(
"$ENV{X_LINK_DIR}"
"$ENV{VCPKG_LINK_DIR}"
)

qt_add_executable(${PROJECT_NAME}
"src/game/Asteroids.cpp" 
"src/game/AsteroidsConsumers.cpp" 
"src/game/Bullet.cpp"
"src/game/Rock.cpp" 
"src/game/Ship.cpp" 
"src/gl/GL.cpp"
"src/gl/GLBackend.cpp"
"src/gl/GLBackendEmitters.cpp"
"src/gl/GLEntity.cpp" 
"src/gl/GLEntityTask.cpp"
# Add Qt resources file
"include/gl/GL.h"
"include/gl/GLBackend.h"
"resources/GLAsteroids.qrc"
# Add the main
"main.cpp"
)

set(COMMON_DEPENDENCIES
"DatabaseAdapters"
"Entities" 
"Events" 
"FilesystemAdapters" 
"Resources" 
)

if (APPLE)
target_link_libraries(${PROJECT_NAME} PRIVATE
"${COMMON_DEPENDENCIES}" 
"OpenGL::GL"
"boost_filesystem"
"Qt6::Quick"
"Qt6::QuickControls2"
"Qt6::Widgets"
"Qt6::OpenGL"
"Qt6::OpenGLWidgets"
)
elseif (UNIX)
target_link_libraries(${PROJECT_NAME} PRIVATE
"${COMMON_DEPENDENCIES}" 
"GL"
"boost_filesystem"
"m"
"dl"
"pthread"
"Xi"
"Xrandr"
"Xxf86vm"
)
endif()

# Create an application bundle
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE ON
)

