#include "climbers.h"

#include <iostream>

#include "log.h"
#include "enginestate.h"
#include "splashstate.h"

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
}

void Climbers::Update(float GameTime)
{
	frames.Update(GameTime);
}

void Climbers::Draw()
{
	frames.Draw();
}


