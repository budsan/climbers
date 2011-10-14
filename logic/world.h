#pragma once

#include "audio/emyl.h"
#include "graphics/camera2d.h"

#include "tools/particleemitter.h"

#include "randomtowertilemap.h"
#include "player.h"

#define NPLAYERS 2

class World
{
public:
	 World();
	~World();

	void Update(float GameTime);
	void Draw();

	void   Load();
	void Unload();

	Camera2D camera;
	RandomTower map;
	Player *players[NPLAYERS];

	float posCamY;
	float velCamY;
	bool camMoving;

	std::vector<ParticleEmitter*> emitters;

	//CONTENT
	ParticleEmitter *blockDestroy;
	emyl::stream *music;

	//TODO: Hacerlo por eventos.
	void destroyBlock(int x, int y);
};

