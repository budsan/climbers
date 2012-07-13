#include "actionsclimber.h"

Action guard;

ActionsClimber::ActionsClimber() : Actions(ActionCount)
{

}

void ActionsClimber::onKeyUp(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	getActionFromKey(unicode, key, mods).actionUp();
}

void ActionsClimber::onKeyDown(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{

	getActionFromKey(unicode, key, mods).actionDown();
}

Action& ActionsClimberOne::getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	switch(key)
	{
	case Guy::Keyboard::Key_Up:      return m_actions[Up]; break;
	case Guy::Keyboard::Key_Down:    return m_actions[Down]; break;
	case Guy::Keyboard::Key_Left:    return m_actions[Left]; break;
	case Guy::Keyboard::Key_Right:   return m_actions[Right]; break;
	case Guy::Keyboard::Key_RControl:return m_actions[Jump]; break;
	case Guy::Keyboard::Key_RShift:  return m_actions[Special]; break;
	default: return guard; break;
	}
}

Action& ActionsClimberTwo::getActionFromKey(wchar_t unicode, Guy::Keyboard::Key key, Guy::Keyboard::Mod mods)
{
	switch(key)
	{
	case Guy::Keyboard::Key_W:       return m_actions[Up]; break;
	case Guy::Keyboard::Key_S:       return m_actions[Down]; break;
	case Guy::Keyboard::Key_A:       return m_actions[Left]; break;
	case Guy::Keyboard::Key_D:       return m_actions[Right]; break;
	case Guy::Keyboard::Key_LControl:return m_actions[Jump]; break;
	case Guy::Keyboard::Key_LShift:  return m_actions[Special]; break;
	default: return guard; break;
	}
}
