#include "splashstate.h"
#include "menustate.h"

#include "gameframework/game.h"
#include "gameframework/audio/emyl.h"
#include "gameframework/graphics/screen.h"
#include "gameframework/graphics/texturemanager.h"
#include "gameframework/graphics/color.h"

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#define ACUMM_NEW_FRAME_MULT 0.15f
#define ACUMM_OLD_FRAME_MULT (1.0f-ACUMM_NEW_FRAME_MULT)

using namespace std;

SplashState:: SplashState() : m_state(nullptr), m_nextState(nullptr), m_introSound(0)
{

}

SplashState::~SplashState()
{
	unload();
}

void SplashState::load()
{
	if (m_state != nullptr)
	{
		delete m_state;
	}

	m_state = new Starting(*this);

	//LOADING TEXTURES
	TextureManager &texman= TextureManager::instance();
    texman.useTexture("logo");
    texman.useTexture("presents");
    texman.useTexture("gameby");

	if (m_introSound == 0)
	{
		emyl::manager* audiomng = emyl::manager::get_instance();
		m_introSound = audiomng->get_buffer("data/sound/intro.ogg");

		m_soundHandler = new emyl::sound();
		m_soundHandler->set_buffer(m_introSound);
		m_soundHandler->set_source();
	}

	//OPENGL INITS
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_isAccumFirstFrame = true;
}

void SplashState::unload()
{
	TextureManager &texman= TextureManager::instance();
    texman.deleteTexture("logo");
    texman.deleteTexture("presents");
    texman.deleteTexture("gameby");

	if (m_introSound) {
		emyl::manager* mng = emyl::manager::get_instance();
		delete m_soundHandler;
		mng->delete_buffer("data/sound/intro.ogg");
		m_introSound = 0;
	}

	if (m_state != nullptr)
	{
		delete m_state;
		m_state = nullptr;
	}

	//OPENGL RESTAURE
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glClear(GL_ACCUM_BUFFER_BIT);


}

void SplashState::changeState(SplashState::InnerState *next)
{
	if (m_nextState != nullptr)
	{
		delete m_nextState;
	}

	m_nextState = next;
}

void SplashState::update(float deltaTime)
{
	if (m_nextState != nullptr)
	{
		if (m_state != nullptr) delete m_state;
		m_state = m_nextState;
		m_nextState = nullptr;
	}

	m_state->update(deltaTime);
}

void SplashState::draw()
{
	m_state->draw();
}

//-STATE---------------------------------------------------------------------//

SplashState::InnerState::InnerState(SplashState &m_splash) : m_splash(m_splash), m_timeCount(0)
{
    m_texMan = TextureManager::ptrInstance();
	m_audio = emyl::manager::get_instance();
}

void SplashState::InnerState::update(float deltaTime)
{
	inUpdate(deltaTime);
	m_timeCount += deltaTime;
}

void SplashState::InnerState::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	inDraw();

    Screen::instance().fillWithColor(m_fadeColor);
	if (m_splash.m_isAccumFirstFrame) m_splash.m_isAccumFirstFrame = false;
	else
	{
		glAccum(GL_ACCUM, ACUMM_NEW_FRAME_MULT);
		glAccum(GL_RETURN, 1.f);
	}

	glClear(GL_ACCUM_BUFFER_BIT);
	glAccum(GL_ACCUM, ACUMM_OLD_FRAME_MULT);
}

//-STARTING-------------------------------------------------------------------//

SplashState::Starting::Starting(SplashState &splash) : InnerState(splash)
{
	m_fadeColor = rgba(0,0,0,1);
}

void SplashState::Starting::inUpdate(float deltaTime)
{
	// De 0,7segs a 1seg hacemos fade
	if(m_timeCount > 0.7f && m_timeCount < 0.999f)
	{
		m_fadeColor = rgba(1.0f - (1.0f - m_timeCount)/(1.0f - 0.7f));
		m_fadeColor.a = 1.0f; //A
	}

	// Pasado el segundo, cambiamos de estado y el fade a blanco
	if(m_timeCount >= 1.0f)
	{
		m_fadeColor = rgba(1.0f);
        m_splash.changeState(new SplashState::Presents(m_splash));
		m_splash.m_soundHandler->play();
	}
}

void SplashState::Starting::inDraw()
{

}

//-PRESENTS------------------------------------------------------------------//

SplashState::Presents::Presents(SplashState &splash) : InnerState(splash)
{
	m_fadeColor = rgba(1.0f);
}

void SplashState::Presents::inUpdate(float deltaTime)
{
	// Fade out de blanco a transparente
	if(m_timeCount < 0.3f) {
		m_fadeColor.a = (0.3f - m_timeCount)/(0.3f);
	}
	//Esperamos un 0.3s a 3,7s a que se muestre el PRESENTS
	else if(m_timeCount >= 0.3f && m_timeCount <= 3.5f) {
		m_fadeColor.a = 0.0f;
	}
	//Fade in a blanco de 3 milisegundos
	else if(m_timeCount > 3.5f && m_timeCount < 3.799f) {
		m_fadeColor.a = 1.0f - (3.8f - m_timeCount)/(3.8f - 3.5f);
	}
	//Y finalmente cambiamos el estado y el fade a blanco
	else if(m_timeCount >= 3.8f) {
		m_fadeColor.a = 1.0f;
        m_splash.changeState(new SplashState::Gameby(m_splash));
	}
}

void SplashState::Presents::inDraw()
{
	Screen &gfx = Screen::instance();
    gfx.fillWithColor(rgba(1,1,1,1));
	float ratio = gfx.getRatio();

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	float fov = 60 -(m_timeCount*m_timeCount/16.0f)*10;
	gluPerspective(fov,ratio, 0.1, 150.0);

	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	gluLookAt(-1.50f, 1.50f, 4.00f,
		  -0.25f, -(m_timeCount*m_timeCount/80), 0.00f,
		   0.00f, 1.00f, 0.00f);

    m_texMan->useTexture("logo");
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	 //VGAFIB LOGO
	 glColor4f(1, 1, 1, 1);
	 glTexCoord2f(0.0f, 0.0f); glVertex3f(-2, 0, 0);
	 glTexCoord2f(1.0f, 0.0f); glVertex3f( 2, 0, 0);
	 glTexCoord2f(1.0f, 1.0f); glVertex3f( 2, 1, 0);
	 glTexCoord2f(0.0f, 1.0f); glVertex3f(-2, 1, 0);

	 //VGAFIB LOGO SHADOW
	 glColor4f(1, 1, 1, 0.5f); glTexCoord2f(0, 0); glVertex3f(-2, 0.03f, 0);
	 glColor4f(1, 1, 1, 0.5f); glTexCoord2f(1, 0); glVertex3f( 2, 0.03f, 0);
	 glColor4f(1, 1, 1, 0); glTexCoord2f(1, 0.8f); glVertex3f( 2, 0.03f, 1);
	 glColor4f(1, 1, 1, 0); glTexCoord2f(0, 0.8f); glVertex3f(-2, 0.03f, 1);

	glEnd();

	//PRESENTS
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho(-ratio/2.0f, ratio/2.0f, -0.5f, 0.5f, -1, 1);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	glTranslatef(0,-0.3175f,-0.5f);

	const Texture &presents = m_texMan->getTexture("presents");
	presents.setFiltering(GL_NEAREST);
	presents.setWrap(GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba(1,1,1,1).raw());

	//VARIABLE DISTANCE BETWEEN LETTERS
	//EACH ARE SIZED (1/16)x(1/16), 0.0f <= SPLASH_TIMECOUNT <= 4.0f
	const float stride = (m_timeCount*m_timeCount)/(16.0f * 9.0f);
	float pos;

	//CENTER LETTERS, STABLISH INITIAL POSSITION.
	//-((INDEX * NUM_SPACES_BETWEEN_LETTERS) + PRESENTS_LENGHT)/2.0f
	pos = -(stride*7.0f + 0.5f) * 0.5f;

	//DRAW SHADE
	glBegin(GL_QUADS);
	for(int i = 0; i < 8; i++)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
		glTexCoord2f((i+0.0f)/8.f,0); glVertex3f(pos, 0, 0);
		glTexCoord2f((i+1.0f)/8.f,0); glVertex3f(pos+(1.f/16.f), 0, 0);
		glTexCoord2f((i+1.0f)/8.f,1); glVertex3f(pos+(1.f/16.f), 1.f/16.f, 0);
		glTexCoord2f((i+0.0f)/8.f,1); glVertex3f(pos, 1.f/16.f , 0);
		pos += (stride + 1.0f/16.0f); //LETTER LENGHT + INDEX
	}
	glEnd();

	//REINIT POS
	pos = -(stride*7.0f + 0.5f) * 0.5f;

	//DRAW PRESENTS
	glTranslatef(-0.005f, 0.005f, 0);
	glBegin(GL_QUADS);
	for(int i = 0; i < 8; i++)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTexCoord2f((i+0.0f)/8.f,0); glVertex3f(pos, 0, 0);
		glTexCoord2f((i+1.0f)/8.f,0); glVertex3f(pos+(1.f/16.f), 0, 0);
		glTexCoord2f((i+1.0f)/8.f,1); glVertex3f(pos+(1.f/16.f), 1.f/16.f, 0);
		glTexCoord2f((i+0.0f)/8.f,1); glVertex3f(pos, 1.f/16.f , 0);
		pos += (stride + 1.0f/16.0f);
	}
	glEnd();
}

//-GAMEBY--------------------------------------------------------------------//

SplashState::Gameby::Gameby(SplashState &splash) : InnerState(splash)
{
    m_fadeColor = rgba(1.0f);
}

void SplashState::Gameby::inUpdate(float deltaTime)
{
	//Fade out de blanco a transparente de 0.6s
	if(m_timeCount <= 0.6f) {
		m_fadeColor.a = (0.6f - m_timeCount)/(0.6f); //A
	}
	//mostramos GAMEBY de 0.3s a 4.4s
	else if(m_timeCount >= 0.3f && m_timeCount <= 4.4f) {
		m_fadeColor.a = 0.0f; //A
	}
	//Fade in de transparente a negro de 0.6s
	else if(m_timeCount > 4.4f && m_timeCount < 5.0f) {
		m_fadeColor   = rgba(0.0f);
		m_fadeColor.a = 1.0f - (5.0f - m_timeCount)/(5.0f - 4.4f); //A
	}
	//Y finalmente hemos acabado y cambiamos de estado.
	else if(m_timeCount >= 5.0f) {
		m_fadeColor.a = 1.0f; //A
        m_splash.changeState(new SplashState::Ending(m_splash));
	}
}

void SplashState::Gameby::inDraw()
{
	Screen &gfx = Screen::instance();
    gfx.fillWithColor(rgba(1,1,1,1));
	float ratio = gfx.getRatio();

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-ratio, ratio, -1.0f, 1.0f, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    m_texMan->useTexture("gameby");

	glBegin(GL_QUADS);
	 glColor4f(1, 1, 1, 1);
	 glTexCoord2f(0, 0); glVertex3f(-1.2f,-0.6f, -0.5f);
	 glTexCoord2f(1, 0); glVertex3f( 1.2f,-0.6f, -0.5f);
	 glTexCoord2f(1, 1); glVertex3f( 1.2f, 0.6f, -0.5f);
	 glTexCoord2f(0, 1); glVertex3f(-1.2f, 0.6f, -0.5f);
	glEnd();
}

//-ENDING--------------------------------------------------------------------//

SplashState::Ending::Ending(SplashState &splash) : InnerState(splash)
{
    m_fadeColor   = rgba(0.0f, 0.0f, 0.0f, 1.0f);
}

void SplashState::Ending::inUpdate(float deltaTime)
{
	//Esperamos 1 segundo y nos vamos al menu
	if(m_timeCount >= 1.0f)
	{
        m_splash.getGame()->changeState(new MenuState());
	}
}

void SplashState::Ending::inDraw()
{

}
