#include "tilemap.h"
#include <math.h>

Tilemap::Tilemap(float unitsPerTile) : myTileSize(unitsPerTile)
{
}

math::vec2i Tilemap::tilePos(math::vec2f pos)
{
	return tilePos(pos.x, pos.y);
}

math::vec2i Tilemap::tilePos(float x, float y)
{
	return math::vec2i(
                floor(x/myTileSize),
                floor(y/myTileSize));
}

unsigned int Tilemap::tilePosX(float x)
{
        return floor(x/myTileSize);
}

unsigned int Tilemap::tilePosY(float y)
{
        return floor(y/myTileSize);
}

float Tilemap::Top(int y)
{
        return float(y+1)*myTileSize;
}

float Tilemap::Bottom(int y)
{
        return float(y)*myTileSize;
}

float Tilemap::Left(int x)
{
        return float(x)*myTileSize;
}

float Tilemap::Right(int x)
{
        return float(x+1)*myTileSize;
}

