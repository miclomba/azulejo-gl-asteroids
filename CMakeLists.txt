cmake_minimum_required(VERSION 3.19)
project(GLAsteroids LANGUAGES CXX)

# — print environment variables —
message(STATUS "VCPKG_INCL_DIR: $ENV{VCPKG_INCL_DIR}")
message(STATUS "VCPKG_LINK_DIR: $ENV{VCPKG_LINK_DIR}")
message(STATUS "X_INCL_DIR: $ENV{X_INCL_DIR}")
message(STATUS "X_LINK_DIR: $ENV{X_LINK_DIR}")

# — Enable C++20 —
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# — enable automoc, autorcc, autouic for Qt6 —
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# — determine vcpkg triplet based on platform —
if(UNIX AND NOT APPLE)
    set(VCPKG_TRIPLET x64-linux)
elseif(APPLE)
    set(VCPKG_TRIPLET x64-osx)
endif()

message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ flags (Debug): ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags (Release): ${CMAKE_CXX_FLAGS_RELEASE}")

# — include paths —
set(VCPKG_INCL_DIR_ENV $ENV{VCPKG_INCL_DIR})
set(X_INCL_DIR_ENV    $ENV{X_INCL_DIR})
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    /usr/local/include
    ${VCPKG_INCL_DIR_ENV}
    ${X_INCL_DIR_ENV}
)

# — library paths —
set(VCPKG_LINK_DIR_ENV $ENV{VCPKG_LINK_DIR})
set(X_LINK_DIR_ENV    $ENV{X_LINK_DIR})
link_directories(
    /usr/local/lib
    ${VCPKG_LINK_DIR_ENV}
    ${X_LINK_DIR_ENV}
)

# — find Qt6 modules (will pick up system Qt on macOS) —
find_package(Qt6 REQUIRED 
PATHS
    "$ENV{HOME}/Qt/6.11.0/macos"
COMPONENTS
    Core
    Gui
    Widgets
    Quick
    QuickControls2
    OpenGL
    OpenGLWidgets
    Qml
)

# — sources —
set(SRC_FILES
    main.cpp
    src/game/Asteroids.cpp
    src/game/AsteroidsConsumers.cpp
    src/game/Bullet.cpp
    src/game/Rock.cpp
    src/game/Ship.cpp
    src/gl/GL.cpp
    src/gl/GLBackend.cpp
    src/gl/GLBackendEmitters.cpp
    src/gl/GLEntity.cpp
    src/gl/GLEntityTask.cpp
)

# — headers —
set(HEADER_FILES
    include/configuration/config.h
    include/configuration/filesystem.h
    include/configuration/filesystem.hpp
    include/configuration/serialization.h
    include/game/Asteroids.h
    include/game/AsteroidsConsumers.h
    include/game/Bullet.h
    include/game/Rock.h
    include/game/Ship.h
    include/gl/GL.h
    include/gl/GLBackend.h
    include/gl/GLBackendEmitters.h
)

# — resources —
qt6_add_resources(RESOURCE_FILES resources/GLAsteroids.qrc)

# — executable —
add_executable(${PROJECT_NAME}
    ${SRC_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
)

# — link Qt —
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    Qt6::Qml
)

# — link custom libs —
target_link_libraries(${PROJECT_NAME} PRIVATE
    DatabaseAdapters
    Entities
    Events
    FilesystemAdapters
    Resources
    boost_filesystem
)

# — platform-specific linkage —
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        GL
        m
        dl
        pthread
        Xi
        Xrandr
        Xxf86vm
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
    )
endif()

# — QML import path —
set_target_properties(${PROJECT_NAME} PROPERTIES
    QT_QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/qml"
)
