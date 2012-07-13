include(guyframework/guyframework.pri)
TARGET = climbers

SOURCES += \
    main.cpp \
    logic/world.cpp \
    logic/tilemap.cpp \
    logic/player.cpp \
    logic/tilemapcharacter.cpp \
    climbers.cpp \
    logic/randomtowertilemap.cpp \
    action.cpp \
    actionsclimber.cpp
    
HEADERS += \
    climbers.h \
    logic/randomtowertilemap.h \
    logic/world.h \
    logic/tilemapcharacter.h \
    logic/tilemap.h \
    logic/player.h \
    action.h \
    actionsclimber.h

