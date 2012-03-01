#include "randomtowertilemap.h"

#include <cstdlib>
#include <random>

#include "graphics/primitives.h"
#include "graphics/color.h"

#define RESTORE_TIME_DELETED_TILES 5.0f

RandomTower::RandomTower(float unitsPerTile) : Tilemap(unitsPerTile)
{
}

void RandomTower::init(int seed, int wide)
{
	m_sizes = math::vec2i(wide, 40);
	this->m_seed = seed;

	m_tiles.clear();
	m_rowsid.clear();
	int total = m_sizes.x * m_sizes.y;
	m_tiles.assign(total, false);
	m_rowsid.assign(m_sizes.y, -1);
}

void RandomTower::setColl(int x, int y, bool col)
{
	if ( y < 0 ) return;
	if ( x < 0  || x >= m_sizes.x ) return;

	int row = y%m_sizes.y;
	if (m_rowsid[row] == y ) set(x, row, col);
	addDeletedTile(x, y);
}

bool RandomTower::isColl(int x, int y)
{
	if ( y < 0 ) return true;
	if ( x < 0  || x >= m_sizes.x ) return false;
	if(!testRowY(y)) rebuildRowY(y);

	return get(x, y%m_sizes.y);
}

void RandomTower::update(float deltaTime)
{
	updateDeletedTiles(deltaTime);
}

void RandomTower::draw(const math::bbox2f &screen)
{
	math::vec2i start = tilePos(screen.min);
	math::vec2i end   = tilePos(screen.max) + math::vec2i(1,1);

	for (int j = start.y; j < end.y; ++j)
	{
		drawDeletedTiles(j);
		for (int i = start.x; i < end.x; ++i)
		{
			math::bbox2f quad(
						math::vec2f(i,j)*m_tileSize,
						math::vec2f(i+1,j+1)*m_tileSize);

			if (isColl(i,j))
			{
				glColor(rgb(0.400f, 0.275f, 0.195f));
                ::draw(quad);
			}
		}
	}
}

bool RandomTower::testRowY(int y)
{
	int row = y % m_sizes.y;
	return m_rowsid[row] == y;
}

void RandomTower::rebuildRowY(int y)
{
	int row = y % m_sizes.y;
	m_rowsid[row] = y;
	for (int i = 0; i < m_sizes.x; i++) set(i, row, false);
	unsigned int ydiv = y/5, ymod = y%5;
	unsigned long yseed = ((unsigned long)(ydiv))<<16|((unsigned long)(m_seed&0xFFFF));
	std::mt19937 random(yseed);
	if (ymod == (3+random()%2))
	{
		int x = 0;
		int xmax = m_sizes.x - 3;
		while (x < xmax)
		{
			x += (random()%16) + 3;
			int l = x + (random()%3) + 3;
			if ( l >= xmax ) break;
			while (x < l)
			{
				set(x, row, true);
				x++;
			}

			//REDESTROY TILES
			std::map<int, std::map<int, float> >::iterator line = m_deletedTiles.find(y);
			if (line != m_deletedTiles.end())
			{
				std::map<int, float>::iterator it = line->second.begin();
				for (;it != line->second.end(); it++)
				{
					set(it->first, row, false);
				}
			}
		}
	}
}

void RandomTower::addDeletedTile(int x, int y)
{
	m_deletedTiles[y][x] = RESTORE_TIME_DELETED_TILES;
}

void RandomTower::updateDeletedTiles(float deltaTime)
{
	std::map<int, std::map<int, float> >::iterator it1 = m_deletedTiles.begin();
	while(it1 != m_deletedTiles.end())
	{
		std::map<int, float>::iterator it2 = it1->second.begin();
		while (it2 != it1->second.end())
		{
			it2->second -= deltaTime;
			if (it2->second < 0)
			{
				int y = it1->first; int row = y%m_sizes.y;
				if (m_rowsid[row] == y) set(it2->first, row, true);
				std::map<int, float>::iterator toDelete = it2++;
				it1->second.erase(toDelete);
			}
			else it2++;
		}

		if (it1->second.empty())
		{
			std::map<int, std::map<int, float> >::iterator toDelete = it1++;
			m_deletedTiles.erase(toDelete);
		}
		else it1++;
	}
}

void RandomTower::drawDeletedTiles(int y)
{
	//DELETED TILES
	std::map<int, std::map<int, float> >::iterator line = m_deletedTiles.find(y);
	if (line != m_deletedTiles.end())
	{
		std::map<int, float>::iterator it = line->second.begin();
		for (;it != line->second.end(); it++)
		{
			if (it->second <= 1.0f)
			{
				int i = it->first;
				int j = y;
				math::bbox2f quad(
							math::vec2f(i,j)*m_tileSize,
							math::vec2f(i+1,j+1)*m_tileSize);

				glColor(rgba(0.400f, 0.275f, 0.195f, 1.0f - it->second));
				draw(quad);
			}
		}
	}
}

void RandomTower::set(int i, int j, bool col)
{
	m_tiles[j*m_sizes.x + i] = col;
}

bool RandomTower::get(int i, int j)
{
	return m_tiles[j*m_sizes.x + i];
}
