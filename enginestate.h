#pragma once

#include "gameframework/gamestate.h"
#include "logic/world.h"

class EngineState : public GameState
{
public:
	 EngineState();
	~EngineState();

	void Update(float GameTime);
	void Draw();
	
	void Load();
	void Unload();

private:

	World myWorld;
};

