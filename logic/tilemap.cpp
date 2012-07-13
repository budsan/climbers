#include "tilemap.h"
#include <math.h>

Tilemap::Tilemap(float unitsPerTile) : m_unitsPerTile(unitsPerTile)
{
}

math::vec2i Tilemap::tilePos(math::vec2f pos)
{
	return tilePos(pos.x, pos.y);
}

math::vec2i Tilemap::tilePos(float x, float y)
{
	return math::vec2i(
		(int) floor(x/m_unitsPerTile),
		(int) floor(y/m_unitsPerTile));
}

int Tilemap::tilePosX(float x)
{
	return (int) floor(x/m_unitsPerTile);
}

int Tilemap::tilePosY(float y)
{
	return (int) floor(y/m_unitsPerTile);
}

float Tilemap::Top(int y)
{
	return float(y+1)*m_unitsPerTile;
}

float Tilemap::Bottom(int y)
{
	return float(y)*m_unitsPerTile;
}

float Tilemap::Left(int x)
{
	return float(x)*m_unitsPerTile;
}

float Tilemap::Right(int x)
{
	return float(x+1)*m_unitsPerTile;
}

