cmake_minimum_required(VERSION "3.12") #"3.12.18081601-MSVC_2"
#set(CMAKE_GENERATOR "Visual Studio 15 2017 Win64")

#set(CMAKE_CXX_COMPILER "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.16.27023/bin/Hostx64/x64/cl.exe")
#set(CMAKE_C_COMPILER "${CMAKE_CXX_COMPILER}")
#file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/build") 

project("GLAsteroids")


# Setup the bitness of the system
set(CMAKE_CXX_STANDARD 17)

message(STATUS "System: $ENV{PATH}")
if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
#	set(X_64BIT ON)
#	add_definitions(-DBIT64)
#else()
#	fatal_error("X project only supports 64 bit systemss")
endif()

# Find out the OS
#if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#	fatal_error("X project doesn't yet support Linux")
#elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
#	add_definitions(-DWIN32)
#else()
#	fatal_error("X project doesn't support system: ${CMAKE_SYSTEM_NAME}")
#endif()

#info(STATUS "System: ${CMAKE_SYSTEM_NAME}")

# Find out compiler
#if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
#elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
#else()
#	fatal_error("Unknown compiler: ${CMAKE_CXX_COMPILER_ID}")

# Build types
#if (MSVC)
#	set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE INTERNAL "Build configurations for MSVC" FORCE)
#else()
#	set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE INTERNAL "" FORCE)
#	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
#endif()

# Compiler
#if (UNIX)
#elseif(WIN32)
#	set(SYSTEM_LIBS )
#endif()

include_directories(
"../../Backend/X"
"C:/Program Files/boost/boost_1_69_0"
"C:/Program Files/freeglut/include"
)

#cmake -G "Visual Studio 15 2017 Win64" --configure ..

link_directories(
"C:/Users/miclomba/source/repos/Backend/X/x64/Debug"
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

#set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/"build")
#set(CMAKE_CURRENT_BINARY_DIR ${CMAKE_BINARY_DIR})
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

#set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
#install(TARGETS ${PROJECT_NAME} DESTINATION bin)  #${CMAKE_INSTALL_PREFIX}/bin

