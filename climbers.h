#pragma once
#include "game.h"
#include "graphics/tools/frameshud.h"

class Climbers : public Game
{
public:
    Climbers();

protected:
    void Configure();
    void Update(float GameTime);
    void Draw();

    FramesHUD frames;
};

