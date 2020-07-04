cmake_minimum_required(VERSION "3.12") 

project("GLAsteroids")

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
set(CMAKE_CXX_FLAGS "/permissive- /EHsc")
endif()

include_directories(
"$ENV{X_INCL_DIR}"
"$ENV{BOOST_INCL_DIR}"
"$ENV{FREEGLUT_INCL_DIR}"
)

link_directories(
"$ENV{X_LINK_DIR}"
"$ENV{BOOST_LINK_DIR}" 
"$ENV{FREEGLUT_LINK_BIN}"
"$ENV{FREEGLUT_LINK_LIB}"
)

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

target_link_libraries("${PROJECT_NAME}" 
"freeglutd" 
"Entities" 
"Resources" 
"Events" 
"FilesystemAdapters" 
"DatabaseAdapters"
)

