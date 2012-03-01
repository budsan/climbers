#include "enginestate.h"
#include "gameframework/graphics/graphics.h"

EngineState:: EngineState() {}
EngineState::~EngineState() {}

void EngineState::update(float deltaTime)
{
	m_world.update(deltaTime);
}

void EngineState::load()
{
	m_world.load();
}

void EngineState::unload()
{
	m_world.unload();
}

void EngineState::draw()
{
	m_world.draw();
}

