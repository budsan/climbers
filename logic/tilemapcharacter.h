#pragma once

#include "graphics/spriteanim.h"
#include "math/algebra3.h"

#include "tilemap.h"

class TilemapCharacter : public Guy::SpriteAnim
{
public:
	TilemapCharacter(Tilemap &m_parent);
	virtual void update(float deltaTime);
private:
	math::vec2f siz;
	math::vec2f cen;

protected:
	math::vec2f m_acc;
	math::vec2f m_vel;
	math::vec2f m_fri;
	math::vec2f m_velLim;
	float m_animVelFactor;

	bool ensureAnim(std::string name);

	virtual void noLeftCollision();
	virtual void noRightCollision();
	virtual void noUpCollision();
	virtual void noDownCollision();

	virtual bool onLeftCollision(int x, int j);
	virtual bool onRightCollision(int x, int j);
	virtual bool onUpCollision(int x, int j);
	virtual bool onDownCollision(int x, int j);

	std::string m_lastAnim;
	Tilemap &m_parent;

	virtual void preDrawing();
};
