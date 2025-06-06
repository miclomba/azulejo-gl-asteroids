cmake_minimum_required(VERSION "3.12") 

project("GLAsteroids")

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

message(STATUS "FREEGLUT_LINK_BIN: $ENV{FREEGLUT_LINK_BIN}")
message(STATUS "FREEGLUT_LINK_LIB: $ENV{FREEGLUT_LINK_LIB}")
message(STATUS "FREEGLUT_INCL_DIR: $ENV{FREEGLUT_INCL_DIR}")
message(STATUS "BOOST_INCL_DIR: $ENV{BOOST_INCL_DIR}")
message(STATUS "BOOST_LINK_DIR: $ENV{BOOST_LINK_DIR}")
message(STATUS "X_INCL_DIR: $ENV{X_INCL_DIR}")
message(STATUS "X_LINK_DIR: $ENV{X_LINK_DIR}")
message(STATUS "X11_LIBRARIES: ${X11_LIBRARIES}")
message(STATUS "X11_INCLUDE_DIR: ${X11_INCLUDE_DIR}")

set(COMMON_INCLUDES
"${CMAKE_SOURCE_DIR}/include"
"$ENV{X_INCL_DIR}"
"$ENV{BOOST_INCL_DIR}"
"$ENV{FREEGLUT_INCL_DIR}"
)

set(COMMON_LINK_DIRS
"$ENV{X_LINK_DIR}"
"$ENV{BOOST_LINK_DIR}" 
"$ENV{FREEGLUT_LINK_BIN}"
"$ENV{FREEGLUT_LINK_LIB}"
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

add_executable("${PROJECT_NAME}" 
"src/game/Asteroids.cpp" 
"src/game/Bullet.cpp"
"src/game/Rock.cpp" 
"src/game/Ship.cpp" 
"src/gl/GL.cpp"
"src/gl/GLBackend.cpp"
"src/gl/GLBackendEmitters.cpp"
"src/gl/GLWindow.cpp"
"src/gl/GLEntity.cpp" 
"src/gl/GLEntityTask.cpp"
"main.cpp"
)

set(COMMON_DEPENDENCIES
"DatabaseAdapters"
"Entities" 
"Events" 
"FilesystemAdapters" 
"Resources" 
)

if (MSVC)
target_link_libraries("${PROJECT_NAME}" 
"${COMMON_DEPENDENCIES}" 
"bcrypt"
"boost_filesystem$ENV{BOOST_LIB_SUFFIX}"
"freeglutd"
)
elseif (APPLE)
target_link_libraries("${PROJECT_NAME}" 
"${COMMON_DEPENDENCIES}" 
"GLUT::GLUT" 
"OpenGL::GL"
"boost_filesystem$ENV{BOOST_LIB_SUFFIX}"
)
elseif (UNIX)
target_link_libraries("${PROJECT_NAME}" 
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
