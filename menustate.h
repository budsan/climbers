#pragma once

#include "gameframework/gamestate.h"

class MenuState : public GameState
{
public:
	 MenuState();
	~MenuState();

	void update(float deltaTime);
	void draw();

	void load();
	void unload();

private:

};

