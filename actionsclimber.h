#pragma once

#include "action.h"
#include "guyframework/input/keyboardlistener.h"

class ActionsClimber : public Actions, public Guy::KeyboardListener
{
public:
	ActionsClimber();

	enum Type { Up, Down, Left, Right, Jump, Special, ActionCount };

	void onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
	void onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
	virtual Action& getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods) = 0;
};

class ActionsClimberOne : public ActionsClimber
{
public:
	virtual Action& getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
};

class ActionsClimberTwo : public ActionsClimber
{
public:
	virtual Action& getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods);
};


