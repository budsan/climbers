#include "tilemap.h"
#include <math.h>

Tilemap::Tilemap(float unitsPerTile) : unitsPerTile(unitsPerTile)
{
}

math::vec2i Tilemap::tilePos(math::vec2f pos)
{
	return tilePos(pos.x, pos.y);
}

math::vec2i Tilemap::tilePos(float x, float y)
{
	return math::vec2i(
		floor(x/unitsPerTile),
		floor(y/unitsPerTile));
}

unsigned int Tilemap::tilePosX(float x)
{
	return floor(x/unitsPerTile);
}

unsigned int Tilemap::tilePosY(float y)
{
	return floor(y/unitsPerTile);
}

float Tilemap::Top(int y)
{
	return float(y+1)*unitsPerTile;
}

float Tilemap::Bottom(int y)
{
	return float(y)*unitsPerTile;
}

float Tilemap::Left(int x)
{
	return float(x)*unitsPerTile;
}

float Tilemap::Right(int x)
{
	return float(x+1)*unitsPerTile;
}

