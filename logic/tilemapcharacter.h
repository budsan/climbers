#pragma once

#include "graphics/spriteanim.h"
#include "math/vec2.h"

#include "tilemap.h"

class TilemapCharacter : public SpriteAnim
{
public:
	TilemapCharacter(Tilemap &parent);
	virtual void Update(float GameTime);
private:
	math::vec2f siz;
	math::vec2f cen;

protected:
	math::vec2f acc;
	math::vec2f vel;
        math::vec2f fri;
	math::vec2f vel_lim;
	float animVelFactor;

	void ensureAnim(std::string name);

	virtual void noLeftCollision();
	virtual void noRightCollision();
	virtual void noUpCollision();
	virtual void noDownCollision();

	virtual bool onLeftCollision(int x, int j);
	virtual bool onRightCollision(int x, int j);
	virtual bool onUpCollision(int x, int j);
	virtual bool onDownCollision(int x, int j);

	std::string lastAnim;
	Tilemap &parent;

	virtual void preDrawing();
};
