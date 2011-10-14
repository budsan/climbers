include(gameframework/gameframework.pri)
TARGET = climbers

SOURCES += \
    enginestate.cpp \
    main.cpp \
    logic/world.cpp \
    logic/tilemap.cpp \
    logic/player.cpp \
    logic/tilemapcharacter.cpp \
    climbers.cpp \
    splashstate.cpp \
    menustate.cpp
    
HEADERS += \
    enginestate.h \
    climbers.h \
    splashstate.h \
    menustate.h
