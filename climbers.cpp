#include "climbers.h"

#include <iostream>

#include "log.h"
#include "enginestate.h"
#include "splashstate.h"
#include "keys.h"

#include "settings.h"

Climbers::Climbers()
{

}

void Climbers::Configure()
{
	setFramesPerSecond(0);
	setStableGameTime(false);
	ChangeState(new EngineState());

	if (!frames.LoadFont("data/font/SketchRockwell-Bold.ttf"))
	{
		LOG << "ERROR: Loading frames font" << std::endl;
	}

	Keybinds kb(NUMPLAYERS, K_SIZE);
	kb[0][K_LEFT ].setDefault(SDLK_LEFT);
	kb[0][K_RIGHT].setDefault(SDLK_RIGHT);
	kb[0][K_DOWN ].setDefault(SDLK_DOWN);
	kb[0][K_UP   ].setDefault(SDLK_UP);
	kb[0][K_JUMP ].setDefault(SDLK_END);
	kb[0][K_SPECIAL].setDefault(SDLK_DELETE);

	kb[1][K_LEFT ].setDefault(SDLK_a);
	kb[1][K_RIGHT].setDefault(SDLK_d);
	kb[1][K_DOWN ].setDefault(SDLK_s);
	kb[1][K_UP   ].setDefault(SDLK_w);
	kb[1][K_JUMP ].setDefault(SDLK_h);
	kb[1][K_SPECIAL].setDefault(SDLK_g);

	mySettings->setKeybinds(kb);
}

void Climbers::Update(float GameTime)
{
	frames.Update(GameTime);
}

void Climbers::Draw()
{
	frames.Draw();
}


