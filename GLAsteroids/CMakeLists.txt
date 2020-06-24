cmake_minimum_required(VERSION "3.12") 

project("GLAsteroids")

set(CMAKE_CXX_STANDARD 17)

message(STATUS "System: $ENV{PATH}")

include_directories(
"../../Backend/X"
"C:/Program Files/boost/boost_1_69_0"
"C:/Program Files/freeglut/include"
)

link_directories(
"$ENV{X_LINK_DIR}"
"C:/Program Files/boost/boost_1_69_0/stage/lib" 
"C:/Program Files/freeglut/bin"
"C:/Program Files/freeglut/lib"
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

