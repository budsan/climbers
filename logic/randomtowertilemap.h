#pragma once

#include <vector>
#include <map>
#include "math/vec2.h"
#include "math/bbox.h"

#include "tilemap.h"

class World;
class RandomTower : public Tilemap
{
public:
        RandomTower(float m_tileSize);

    void init(int m_seed, int wide);
	void update(float deltaTime);
	void draw(const math::bbox2f &screen);

	void setColl(int x, int y, bool col); //set tile collisionable
	bool  isColl(int x, int y); //is collisionable

private:
    math::vec2i m_sizes;
    int m_seed;
    std::vector<bool> m_tiles;
    std::vector<int> m_rowsid;

	//MAP < CORD Y, MAP < CORD X, TIMELEFT > >;
    std::map<int, std::map<int, float> > m_deletedTiles;

	void addDeletedTile(int x, int y);
	void updateDeletedTiles(float deltaTime);
	void drawDeletedTiles(int y);

	bool testRowY(int y);
        void rebuildRowY(int y);

	void set(int x, int y, bool col); //set tile collisionable
	bool get(int i, int j);
};
