#include "world.h"
#include "graphics/graphics.h"
#include "graphics/screen.h"
#include "tools/transition.h"

#include <fstream>

#define TILE_SIZE 16
#define TOWER_WIDE 40

World::World() : map(TILE_SIZE), blockDestroy(NULL), music(NULL)
{
	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		players[i] = new Player(*this, i);
	}
}

World::~World()
{
	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		delete players[i];
	}
}

void World::Update(float GameTime)
{
	if (camMoving)
	{
		if (!music->playing())
		{
			music->play();
		}

		posCamY += velCamY * GameTime;
	}

	map.Update(GameTime);

	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		float playerRef = players[i]->position().y - 150.0f;
		if (posCamY < playerRef) {
			posCamY =  playerRef;
			camMoving = true;
		}
		players[i]->Update(GameTime);
	}

	for(unsigned int i = 0; i < emitters.size(); i++)
	{
		ParticleEmitter* emitter = emitters[i];
		emitter->Update(GameTime);
		if (emitter->AreThereParticlesLeft())
		{
			emitters.erase(emitters.begin()+i);
			delete emitter;
			i--;
		}
	}
}

void World::Draw()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Screen &screen = Screen::Instance();
	screen.FillWithColor(rgba(0.200f, 0.100f, 0.100f, 1));

	camera.setPos(math::vec2f(TOWER_WIDE*TILE_SIZE*0.5f, posCamY));
	camera.setOpenGLMatrices();
	map.Draw(camera.getBounding());

	std::vector<ParticleEmitter*>::iterator it = emitters.begin();
	for(; it != emitters.end(); it++) (*it)->Draw();
	for (unsigned int i = 0; i < NPLAYERS; i++) players[i]->Draw();
}

void World::Load()
{

	if (music == NULL)
	{
		music = new emyl::stream();
		music->set_source();
		music->load("data/sound/melody.ogg");
		music->set_loop(true);
		music->set_volume(0.5f);
	}

	if (blockDestroy == NULL)
	{
		std::ifstream file("data/scripts/destroy_block.emp",
				   std::ifstream::in|
				   std::ifstream::binary);

		if (file.is_open())
		{
			blockDestroy = new ParticleEmitter();
			blockDestroy->read(file);
			blockDestroy->Load();
			file.close();
		}
	}

	Player::Load();
	camera.Init();
	camera.resizeScreen(600);
	posCamY = 150;
	velCamY = 32;
	camMoving = false;

	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		players[i]->position() = math::vec2f(20+(i*20), 20);
		players[i]->Reset();
	}

	map.Init(rand(), TOWER_WIDE);
}

void World::Unload()
{
	if (music != NULL)
	{
		delete music;
		music = NULL;
	}

	if (blockDestroy != NULL)
	{
		blockDestroy->Unload();
		delete blockDestroy;
		blockDestroy = NULL;
	}

	Player::Unload();
}

void World::destroyBlock(int x, int y)
{
	map.setColl(x, y, false);
	if (blockDestroy != NULL)
	{
		ParticleEmitter *emitter = new ParticleEmitter(*blockDestroy);
		emitter->setPosition(math::vec2f((x+0.5)*TILE_SIZE,(y+0.5)*TILE_SIZE));
		emitters.push_back(emitter);
	}
}
