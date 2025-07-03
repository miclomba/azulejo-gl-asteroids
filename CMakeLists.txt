cmake_minimum_required(VERSION "3.15") 

project(GLAsteroids)

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
set(CMAKE_CXX_FLAGS "/permissive- /EHsc /WX")
add_definitions(-D_WIN32_WINNT=0x0A00)
elseif(APPLE)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
elseif(UNIX)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)
endif()

# # Find qt libraries
# find_package(Qt6 REQUIRED COMPONENTS QuickControls2 Widgets OpenGL OpenGLWidgets QmlImportScanner)
# # Enable auto meta object compiler and the resource compiler (for QML support)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

message(STATUS "VCPKG_INCL_DIR: $ENV{VCPKG_INCL_DIR}")
message(STATUS "VCPKG_LINK_DIR: $ENV{VCPKG_LINK_DIR}")
message(STATUS "X_INCL_DIR: $ENV{X_INCL_DIR}")
message(STATUS "X_LINK_DIR: $ENV{X_LINK_DIR}")
message(STATUS "X11_LIBRARIES: ${X11_LIBRARIES}")
message(STATUS "X11_INCLUDE_DIR: ${X11_INCLUDE_DIR}")

set(COMMON_INCLUDES
"${CMAKE_SOURCE_DIR}/include"
"$ENV{X_INCL_DIR}"
"$ENV{VCPKG_INCL_DIR}"
)

set(COMMON_LINK_DIRS
"$ENV{X_LINK_DIR}"
"$ENV{VCPKG_LINK_DIR}"
)

if (MSVC OR APPLE)
include_directories(
"${COMMON_INCLUDES}"
)
elseif(UNIX)
include_directories(
"${COMMON_INCLUDES}"
"${X11_INCLUDE_DIR}"
)
endif()

if (MSVC OR APPLE)
link_directories(
"${COMMON_LINK_DIRS}"
)
elseif(UNIX)
link_directories(
"${COMMON_LINK_DIRS}"
"${X11_LIBRARIES}"
)
endif()

add_executable(${PROJECT_NAME}
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
# "src/gl/GLWindow.cpp"
"main.cpp"
# Add Qt resources file
# "resources/GLAsteroids.qrc"
)

set(COMMON_DEPENDENCIES
"DatabaseAdapters"
"Entities" 
"Events" 
"FilesystemAdapters" 
"Resources" 
)

if (MSVC)
target_link_libraries(${PROJECT_NAME}
"${COMMON_DEPENDENCIES}" 
"bcrypt"
"boost_filesystem"
"freeglutd"
)
elseif (APPLE)
target_link_libraries(${PROJECT_NAME}
"${COMMON_DEPENDENCIES}" 
"GLUT::GLUT" 
"OpenGL::GL"
"boost_filesystem"
# "Qt6::Quick"
# "Qt6::QuickControls2"
# "Qt6::Widgets"
# "Qt6::OpenGL"
# "Qt6::OpenGLWidgets"
)
elseif (UNIX)
target_link_libraries(${PROJECT_NAME} 
"${COMMON_DEPENDENCIES}" 
"glut"
"GL"
"boost_filesystem"
"${X11_LIBRARIES}"
"m"
"dl"
"pthread"
"Xi"
"Xrandr"
"Xxf86vm"
)
endif()

