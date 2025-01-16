cmake_minimum_required(VERSION "3.12") 

project("GLAsteroids")

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
set(CMAKE_CXX_FLAGS "/permissive- /EHsc /WX")
add_definitions(-D_WIN32_WINNT=0x0A00)
elseif(APPLE)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
endif()

if (MSVC)
include_directories(
"$ENV{X_INCL_DIR}"
"$ENV{BOOST_INCL_DIR}"
"$ENV{FREEGLUT_INCL_DIR}"
)
elseif (APPLE)
include_directories(
"$ENV{X_INCL_DIR}"
"$ENV{BOOST_INCL_DIR}"
"${OPENGL_INCLUDE_DIR}"
"${GLUT_INCLUDE_DIR}"
)
endif()

if (MSVC)
link_directories(
"$ENV{X_LINK_DIR}"
"$ENV{BOOST_LINK_DIR}" 
"$ENV{FREEGLUT_LINK_BIN}"
"$ENV{FREEGLUT_LINK_LIB}"
)
elseif (APPLE)
link_directories(
"$ENV{X_LINK_DIR}"
"$ENV{BOOST_LINK_DIR}" 
)
endif()

add_executable("${PROJECT_NAME}" 
"main.cpp" 
"GLGame.cpp" 
"Asteroids.cpp" 
"GLEntity.cpp" 
"GLEntityTask.cpp" 
"Rock.cpp" 
"Ship.cpp" 
"Bullet.cpp"
)

if (MSVC)
target_link_libraries("${PROJECT_NAME}" 
"Entities" 
"Resources" 
"Events" 
"FilesystemAdapters" 
"DatabaseAdapters"
"boost_filesystem$ENV{BOOST_LIB_SUFFIX}"
"freeglutd"
"bcrypt"
)
elseif (APPLE)
target_link_libraries("${PROJECT_NAME}" 
"Entities" 
"Resources" 
"Events" 
"FilesystemAdapters" 
"DatabaseAdapters"
"boost_filesystem$ENV{BOOST_LIB_SUFFIX}"
"${OPENGL_LIBRARIES}"
"${GLUT_LIBRARY}"
)
endif()

