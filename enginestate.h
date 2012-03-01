#pragma once

#include "gameframework/gamestate.h"
#include "logic/world.h"

class EngineState : public GameState
{
public:
	 EngineState();
	~EngineState();

	void update(float deltaTime);
	void draw();
	
	void load();
	void unload();

private:

    World m_world;
};

