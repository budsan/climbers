#pragma once

#include "gameframework/gamestate.h"

class MenuState : public GameState
{
public:
	 MenuState();
	~MenuState();

	void Update(float GameTime);
	void Draw();

	void Load();
	void Unload();

private:

};

