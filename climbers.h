#pragma once
#include "gameframework/game.h"
#include "gameframework/graphics/tools/frameshud.h"

class Climbers : public Game
{
public:
    Climbers();

protected:
    void configure();

    void   load();
    void unload();

    void update(float deltaTime);
    void draw();

    const char *getName();
    const char *getVersion();

    FramesHUD m_frames;
};

