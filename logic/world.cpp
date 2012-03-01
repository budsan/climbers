#include "world.h"
#include "graphics/graphics.h"
#include "graphics/screen.h"
#include "tools/transition.h"

#include <fstream>

#define TILE_SIZE 16
#define TOWER_WIDE 40

World::World() : map(TILE_SIZE), blockDestroy(nullptr), music(nullptr)
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

void World::update(float deltaTime)
{
	if (camMoving)
	{
		if (!music->playing())
		{
			music->play();
		}

		posCamY += velCamY * deltaTime;
	}

	map.update(deltaTime);

	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
        float playerRef = players[i]->pos().y - 150.0f;
		if (posCamY < playerRef) {
			posCamY =  playerRef;
			camMoving = true;
		}
		players[i]->update(deltaTime);
	}

	for(unsigned int i = 0; i < emitters.size(); i++)
	{
		ParticleEmitter* emitter = emitters[i];
		emitter->update(deltaTime);
        if (emitter->areParticlesLeft())
		{
			emitters.erase(emitters.begin()+i);
			delete emitter;
			i--;
		}
	}
}

void World::draw()
{
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Screen &screen = Screen::instance();
    screen.fillWithColor(rgba(0.200f, 0.100f, 0.100f, 1));

	camera.setPos(math::vec2f(TOWER_WIDE*TILE_SIZE*0.5f, posCamY));
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(camera.getProjectionMatrix().v);

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(camera.getModelviewMatrix().v);

	map.draw(camera.getBounding());

	std::vector<ParticleEmitter*>::iterator it = emitters.begin();
	for(; it != emitters.end(); it++) (*it)->draw();
	for (unsigned int i = 0; i < NPLAYERS; i++) players[i]->draw();
}

void World::load()
{

    if (music == nullptr)
	{
		music = new emyl::stream();
		music->set_source();
		music->load("data/sound/melody.ogg");
		music->set_loop(true);
		music->set_volume(0.5f);
	}

    if (blockDestroy == nullptr)
	{
		std::ifstream file("data/scripts/destroy_block.emp",
				   std::ifstream::in|
				   std::ifstream::binary);

		if (file.is_open())
		{
			blockDestroy = new ParticleEmitter();
			blockDestroy->read(file);
			blockDestroy->load();
			file.close();
		}
	}

	Player::load();
	camera.init();
	camera.resizeScreen(600);
	posCamY = 150;
	velCamY = 32;
	camMoving = false;

	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
        players[i]->pos() = math::vec2f(20+(i*20), 20);
        players[i]->reset();
	}

	map.init(rand(), TOWER_WIDE);
}

void World::unload()
{
    if (music != nullptr)
	{
		delete music;
        music = nullptr;
	}

    if (blockDestroy != nullptr)
	{
		blockDestroy->unload();
		delete blockDestroy;
        blockDestroy = nullptr;
	}

	Player::unload();
}

void World::destroyBlock(int x, int y)
{
	map.setColl(x, y, false);
    if (blockDestroy != nullptr)
	{
		ParticleEmitter *emitter = new ParticleEmitter(*blockDestroy);
		emitter->setPosition(math::vec2f((x+0.5)*TILE_SIZE,(y+0.5)*TILE_SIZE));
		emitters.push_back(emitter);
	}
}
