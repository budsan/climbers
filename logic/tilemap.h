#pragma once

#include <vector>
#include "math/vec2.h"
#include "math/bbox.h"

class World;
class Tilemap
{
public:
        Tilemap(float unitsPerTile);

	virtual void setColl(int x, int y, bool col) = 0; //set tile collisionable
	virtual bool  isColl(int x, int y) = 0; //is collisionable

	//what tile is in world coordinate
	math::vec2i tilePos(math::vec2f pos); 
	math::vec2i tilePos(float x, float y);
	unsigned int tilePosX(float x);
	unsigned int tilePosY(float y);

	float Top(int y);
	float Bottom(int y);

	float Left(int x);
	float Right(int x);

        float tileSize() {return myTileSize;}

protected:
        float myTileSize;
};
