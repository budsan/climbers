#include "enginestate.h"
#include "graphics/graphics.h"

EngineState:: EngineState() {}
EngineState::~EngineState() {}

void EngineState::Update(float GameTime)
{
	myWorld.Update(GameTime);
}

void EngineState::Load()
{
	myWorld.Load();
}

void EngineState::Unload()
{
	myWorld.Unload();
}

void EngineState::Draw()
{
	myWorld.Draw();
}

