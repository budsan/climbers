#pragma once
#include "guyframework/game.h"
#include "guyframework/graphics/tools/frameshud.h"

#include "logic/world.h"

class Climbers : public Guy::Game, public Guy::FocusListener
{
public:
    Climbers();

protected:
    void init();

    void   load();
    void unload();

    void update(float deltaTime);
    void draw();

    const char *getName();
    const char *getVersion();

    Guy::FramesHUD m_frames;
    World m_world;
};

