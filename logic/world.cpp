#include "world.h"

#include "guyframework/environment.h"
#include "guyframework/graphics/graphics.h"
#include "guyframework/graphics/screen.h"
#include "guyframework/tools/transition.h"

#include <fstream>

static const int TILE_SIZE  = 16;
static const int TOWER_WIDE = 40;

World::World() : m_map(TILE_SIZE), m_blockDestroy(NULL), m_music(NULL)
{
	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		m_players[i] = new Player(*this, i);
	}
}

World::~World()
{
	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		delete m_players[i];
	}
}

void World::update(float deltaTime)
{
	if (m_camMoving)
	{
		if (!m_music->playing())
		{
			m_music->play();
		}

		m_posCamY += m_velCamY * deltaTime;
	}

	m_map.Update(deltaTime);

	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		float playerRef = m_players[i]->pos().y - 150.0f;
		if (m_posCamY < playerRef) {
			m_posCamY =  playerRef;
			m_camMoving = true;
		}
		m_players[i]->update(deltaTime);
	}

	for(unsigned int i = 0; i < m_emitters.size(); i++)
	{
		Guy::ParticleEmitter* emitter = m_emitters[i];
		emitter->update(deltaTime);
		if (emitter->areParticlesLeft())
		{
			m_emitters.erase(m_emitters.begin()+i);
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

	Guy::Screen &screen = Guy::Environment::instance().screen();
	screen.fillWithColor(Guy::rgba(0.200f, 0.100f, 0.100f, 1));

	m_camera.setPos(math::vec2f(TOWER_WIDE*TILE_SIZE*0.5f, m_posCamY));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(m_camera.projectionMatrix().v);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(m_camera.viewMatrix().v);

	m_map.Draw(m_camera.getBounding());

	std::vector<Guy::ParticleEmitter*>::iterator it = m_emitters.begin();
	for(; it != m_emitters.end(); it++) (*it)->draw();
	for (unsigned int i = 0; i < NPLAYERS; i++) m_players[i]->draw();
}

void World::load()
{

	if (m_music == NULL)
	{
		m_music = new emyl::stream();
		m_music->set_source();
		m_music->load("data/sound/melody.ogg");
		m_music->set_loop(true);
		m_music->set_volume(0.5f);
	}

	if (m_blockDestroy == NULL)
	{
		std::ifstream file("data/scripts/destroy_block.emp",
				   std::ifstream::in|
				   std::ifstream::binary);

		if (file.is_open())
		{
			m_blockDestroy = new Guy::ParticleEmitter();
			m_blockDestroy->read(file);
			m_blockDestroy->load();
			file.close();
		}
	}

	Player::load();
	m_camera.init();
	m_camera.resizeScreen(600);
	m_posCamY = 150;
	m_velCamY = 32;
	m_camMoving = false;

	for (unsigned int i = 0; i < NPLAYERS; i++)
	{
		m_players[i]->pos() = math::vec2f(20+(i*20), 20);
		m_players[i]->reset();
	}

	m_map.Init(rand(), TOWER_WIDE);
}

void World::unload()
{
	if (m_music != NULL)
	{
		delete m_music;
		m_music = NULL;
	}

	if (m_blockDestroy != NULL)
	{
		m_blockDestroy->unload();
		delete m_blockDestroy;
		m_blockDestroy = NULL;
	}

	Player::unload();
}

void World::destroyBlock(int x, int y)
{
	m_map.setColl(x, y, false);
	if (m_blockDestroy != NULL)
	{
		Guy::ParticleEmitter *emitter = new Guy::ParticleEmitter(*m_blockDestroy);
		emitter->setPosition(math::vec2f((x+0.5)*TILE_SIZE,(y+0.5)*TILE_SIZE));
		m_emitters.push_back(emitter);
	}
}
