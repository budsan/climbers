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

void Climbers::configure()
{
	setFramesPerSecond(0);
    setStableDeltaTime(false);
    changeState(new EngineState());

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

    m_settings->setKeybinds(k);
    m_settings->get("ScreenWidth" )->set(800);
    m_settings->get("ScreenHeight")->set(600);
    m_settings->get("Fullscreen"  )->set(false);
}

void Climbers::load()
{
    if (!m_frames.loadFont("data/font/SketchRockwell-Bold.ttf"))
	{
		LOG << "ERROR: Loading frames font" << std::endl;
	}
}

void Climbers::unload()
{

}

void Climbers::update(float deltaTime)
{
	m_frames.update(deltaTime);
}

void Climbers::draw()
{
	m_frames.draw();
}


