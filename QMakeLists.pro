# azulejo-gl-asteroids.pro

TEMPLATE   = app
TARGET     = GLAsteroids

# Enable C++17
CONFIG    += c++17

# Qt modules
QT        += core gui widgets quick quickcontrols2 opengl openglwidgets qml

# Include paths (adjust env vars as needed)
INCLUDEPATH += \
    $$PWD/include \
    /usr/local/include \
    $$PWD/../vcpkg/installed/x64-linux/include

# Library paths & linkage
LIBS += \
    -L/usr/local/lib \
    -L$$PWD/../vcpkg/installed/x64-linux/debug/lib \
    -L$$PWD/../vcpkg/installed/x64-linux/lib \
    -lDatabaseAdapters \
    -lEntities \
    -lEvents \
    -lFilesystemAdapters \
    -lResources \
    -lboost_filesystem

unix {
    LIBS += -lGL -lm -ldl -lpthread -lXi -lXrandr -lXxf86vm
}

# Sources
SOURCES += \
    main.cpp \
    src/game/Asteroids.cpp \
    src/game/AsteroidsConsumers.cpp \
    src/game/Bullet.cpp \
    src/game/Rock.cpp \
    src/game/Ship.cpp \
    src/gl/GL.cpp \
    src/gl/GLBackend.cpp \
    src/gl/GLBackendEmitters.cpp \
    src/gl/GLEntity.cpp \
    src/gl/GLEntityTask.cpp

# Headers
HEADERS += \
    include/configuration/config.h \
    include/configuration/filesystem.h \
    include/configuration/filesystem.hpp \
    include/configuration/serialization.h \
    include/game/Asteroids.h \
    include/game/AsteroidsConsumers.h \
    include/game/Bullet.h \
    include/game/Rock.h \
    include/game/Ship.h \
    include/gl/GL.h \
    include/gl/GLBackend.h \
    include/gl/GLBackendEmitters.h

# Qt resource file (pulls in your QML under qml/)
RESOURCES += \
    resources/GLAsteroids.qrc

# (Optional) Make sure the QML import path points at your qml/ folder
QML_IMPORT_PATH += $$PWD/qml

