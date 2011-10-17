#include "climbers.h"

#include <iostream>

#include "gameframework/log.h"
#include "gameframework/settings.h"

#include "enginestate.h"
#include "splashstate.h"
#include "keys.h"

#define GAME_NAME "Climbers"
#define GAME_VERSION "0.1"

Climbers::Climbers()
{

}

const char *Climbers::getName()
{
    return GAME_NAME;
}

const char *Climbers::getVersion()
{
    return GAME_VERSION;
}

void Climbers::Configure()
{
	setFramesPerSecond(0);
	setStableGameTime(false);
	ChangeState(new EngineState());

	Keybinds k(NUMPLAYERS, K_SIZE);
	k[0][K_LEFT ].setDefault(SDLK_LEFT);
	k[0][K_RIGHT].setDefault(SDLK_RIGHT);
	k[0][K_DOWN ].setDefault(SDLK_DOWN);
	k[0][K_UP   ].setDefault(SDLK_UP);
	k[0][K_JUMP ].setDefault(SDLK_END);
	k[0][K_SPECIAL].setDefault(SDLK_DELETE);

	k[1][K_LEFT ].setDefault(SDLK_a);
	k[1][K_RIGHT].setDefault(SDLK_d);
	k[1][K_DOWN ].setDefault(SDLK_s);
	k[1][K_UP   ].setDefault(SDLK_w);
	k[1][K_JUMP ].setDefault(SDLK_h);
	k[1][K_SPECIAL].setDefault(SDLK_g);

	mySettings->setKeybinds(k);
	mySettings->get("ScreenWidth" )->set(800);
	mySettings->get("ScreenHeight")->set(600);
	mySettings->get("Fullscreen"  )->set(false);
}

void Climbers::Load()
{
	if (!frames.LoadFont("data/font/SketchRockwell-Bold.ttf"))
	{
		LOG << "ERROR: Loading frames font" << std::endl;
	}
}

void Climbers::Unload()
{

}

void Climbers::Update(float GameTime)
{
	frames.Update(GameTime);
}

void Climbers::Draw()
{
	frames.Draw();
}


