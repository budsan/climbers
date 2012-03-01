#include "tilemap.h"
#include <math.h>

Tilemap::Tilemap(float unitsPerTile) : m_tileSize(unitsPerTile)
{
}

math::vec2i Tilemap::tilePos(math::vec2f pos)
{
	return tilePos(pos.x, pos.y);
}

math::vec2i Tilemap::tilePos(float x, float y)
{
	return math::vec2i(
                floor(x/m_tileSize),
                floor(y/m_tileSize));
}

unsigned int Tilemap::tilePosX(float x)
{
        return floor(x/m_tileSize);
}

unsigned int Tilemap::tilePosY(float y)
{
        return floor(y/m_tileSize);
}

float Tilemap::top(int y)
{
        return float(y+1)*m_tileSize;
}

float Tilemap::bottom(int y)
{
        return float(y)*m_tileSize;
}

float Tilemap::left(int x)
{
        return float(x)*m_tileSize;
}

float Tilemap::right(int x)
{
        return float(x+1)*m_tileSize;
}

