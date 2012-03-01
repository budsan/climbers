#pragma once

#include "gameframework/gamestate.h"

#include "gameframework/graphics/screen.h"
#include "gameframework/graphics/texturemanager.h"
#include "gameframework/graphics/color.h"

#include "gameframework/audio/emyl.h"

class SplashState : public GameState
{
public:

	 SplashState();
	~SplashState();

	void update(float deltaTime);
	void draw();

	void load();
	void unload();
	
private:

	class InnerState
	{
	public:
        InnerState(SplashState &splash);

		void update(float deltaTime);
		void draw();

	protected:
        virtual void inUpdate(float deltaTime) = 0;
        virtual void inDraw() = 0;

        SplashState &m_splash;
        float m_timeCount;
        rgba m_fadeColor;

        TextureManager* m_texMan;
        emyl::manager* m_audio;
	};

	class Starting : public InnerState
	{
	public:
        Starting(SplashState &splash);

	protected:
        void inUpdate(float deltaTime);
        void inDraw();
	};

	class Presents : public InnerState
	{
	public:
        Presents(SplashState &splash);
	protected:
        void inUpdate(float deltaTime);
        void inDraw();
	};

	class Gameby : public InnerState
	{
	public:
        Gameby(SplashState &splash);
	protected:
        void inUpdate(float deltaTime);
        void inDraw();
	};

	class Ending : public InnerState
	{
	public:
        Ending(SplashState &splash);
	protected:
        void inUpdate(float deltaTime);
        void inDraw();
	};

	friend class InnerState;
	void changeState(SplashState::InnerState *next);

    InnerState *m_state;
    InnerState *m_nextState;

    ALuint m_introSound;
    emyl::sound* m_soundHandler;
    bool m_isAccumFirstFrame;
};
