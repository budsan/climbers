#include "climbers.h"

#include "guyframework/log.h"
#include "actionsclimber.h"

#include <iostream>

#define GAME_NAME "Climbers"
#define GAME_VERSION "0.2"

Climbers::Climbers()
{

}

const char *Climbers::name()
{
    return GAME_NAME;
}

const char *Climbers::version()
{
    return GAME_VERSION;
}

void Climbers::init()
{
	Guy::Environment &env = Guy::Environment::instance();
	env.screen().setMode(Guy::Screen::Mode(800, 600), false);

	Guy::Input &in = env.input();
	in.addFocusListener(this);

	ActionsClimber* actionsClimberOne = new ActionsClimberOne();
	in.keyboard().addListener(actionsClimberOne);

	ActionsClimber* actionsClimberTwo = new ActionsClimberTwo();
	in.keyboard().addListener(actionsClimberTwo);

	std::vector<Actions*> &actions = Actions::instance();
	actions.push_back(actionsClimberOne);
	actions.push_back(actionsClimberTwo);
}

void Climbers::load()
{
	if (!m_frames.loadFont("data/font/SketchRockwell-Bold.ttf"))
	{
		Guy::printLog("ERROR: Loading frames font\n");
	}

	m_world.load();
}

void Climbers::unload()
{
	m_world.unload();
}

void Climbers::update(float deltaTime)
{
	m_world.update(deltaTime);
	m_frames.update(deltaTime);
	Actions::endOfFrameAll();
}

void Climbers::draw()
{
	m_world.draw();
	m_frames.draw();
}


