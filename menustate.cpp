#include "menustate.h"
#include "enginestate.h"

#include "gameframework/game.h"

#include <iostream>

MenuState:: MenuState()
{

}

MenuState:: ~MenuState()
{

}

void MenuState::load()
{

}

void MenuState::unload()
{

}

void MenuState::update(float deltaTime)
{
	getGame()->changeState(new EngineState());
}

void MenuState::draw()
{


}
