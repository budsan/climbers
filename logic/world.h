#pragma once

#include "guyframework/audio/emyl.h"
#include "guyframework/graphics/camera2d.h"
#include "guyframework/tools/particleemitter.h"

#include "randomtowertilemap.h"
#include "player.h"

#define NPLAYERS 2

class World
{
public:
	 World();
	~World();

	void update(float deltaTime);
	void draw();

	void   load();
	void unload();

	Guy::Camera2D m_camera;
	RandomTower m_map;
	Player *m_players[NPLAYERS];

	float m_posCamY;
	float m_velCamY;
	bool m_camMoving;

	std::vector<Guy::ParticleEmitter*> m_emitters;

	//CONTENT
	Guy::ParticleEmitter *m_blockDestroy;
	emyl::stream *m_music;

	//TODO: Do this with signals
	void destroyBlock(int x, int y);
};

