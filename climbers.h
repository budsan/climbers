#pragma once
#include "gameframework/game.h"
#include "gameframework/graphics/tools/frameshud.h"

class Climbers : public Game
{
public:
    Climbers();

protected:
    void Configure();

    void   Load();
    void Unload();

    void Update(float GameTime);
    void Draw();

    const char *getName();
    const char *getVersion();

    FramesHUD frames;
};

