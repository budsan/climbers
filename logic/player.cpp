#include "player.h"

#include "guyframework/audio/emyl.h"
#include "guyframework/log.h"

#include "actionsclimber.h"
#include "world.h"

boost::shared_ptr<Guy::SpriteAnimData> Player::s_sprData[2];
boost::shared_ptr<emyl::sound> Player::s_sndHdl;
ALuint Player::s_sndJump = 0;
ALuint Player::s_sndRoofHit = 0;

Player::Player(World &world, int player) : TilemapCharacter(world.m_map), player(player), world(world)
{
	if (s_sprData[0] == NULL) load();
	setAnimData(s_sprData[player]);
	ensureAnim("Idle");
	jumpTimeLeft = 0.0f;
	crouched = false;
	onWall = ONWALL_NO;
}

void Player::load()
{

	if (s_sprData[0] == NULL)
	{
		s_sprData[0] = boost::shared_ptr < Guy::SpriteAnimData > (new Guy::SpriteAnimData());
		if (!s_sprData[0]->load("data/scripts/mario01.anim"))
		{
			Guy::printLog("CRITICAL: data/scripts/mario01.anim doesn't exist.\n");
		}
	}

	if (s_sprData[1] == NULL)
	{
		s_sprData[1] = boost::shared_ptr < Guy::SpriteAnimData > (new Guy::SpriteAnimData());
		if (!s_sprData[1]->load("data/scripts/mario02.anim"))
		{
			Guy::printLog("CRITICAL: data/scripts/mario02.anim doesn't exist.\n");

		}
	}

	if (s_sndHdl == NULL)
	{
		s_sndHdl = boost::shared_ptr < emyl::sound > (new emyl::sound());
		s_sndHdl->set_source();
	}

	emyl::manager* audiomng = emyl::manager::get_instance();
	if (s_sndJump == 0) s_sndJump = audiomng->get_buffer("data/sound/jump.ogg");
	if (s_sndRoofHit == 0) s_sndRoofHit = audiomng->get_buffer("data/sound/roofhit.ogg");
}

void Player::unload()
{
	s_sprData[0].reset();
	s_sprData[1].reset();
	s_sndHdl.reset();
}

void Player::update(float deltaTime)
{
	Actions &state = *Actions::instance()[player];

	//constantes
	const math::vec2f run_acc(1000,    0); //aceleracion corriendo
	const math::vec2f air_acc( 600,    0); //aceleracion corriendo
	const math::vec2f fri_acc(1000,  1800); //aceleracion de la friccion
	const math::vec2f gra_acc(   0, -1200); //aceleracion de la gravedad
	const math::vec2f vel_max( 150,  300); //velocidad maxima que adquiere un personaje
	const math::vec2f vel_max_run( 300,  300); //velocidad maxima que adquiere un personaje
	const math::vec2f vel_jmp( 300,  300); //velocidad que adquiere un personaje al saltar
	const float jump_time = 0.3f;

	bool idle = false;
	crouched = ((crouched||grounded) && state.isPressed(ActionsClimber::Down)) || (crouched&&!grounded);
	bool ableToWallJump = onWall && !grounded && !crouched&& !walljumped;

	if (crouched)
	{
		ensureAnim("Crouch");
	}
	else
	{
		if (grounded)
		{
			if (state.isPressed(ActionsClimber::Left) && !state.isPressed(ActionsClimber::Right))
			{
				if (m_vel.x > 0) ensureAnim("Turn");
				else ensureAnim("Walk");
			}
			else if (state.isPressed(ActionsClimber::Right) && !state.isPressed(ActionsClimber::Left))
			{
				if (m_vel.x < 0) ensureAnim("Turn");
				else ensureAnim("Walk");
			}
			else
			{
				idle = true;
				ensureAnim("Idle");
			}
		}
		else
		{
			if (ableToWallJump) ensureAnim("OnWall");
			else ensureAnim("Jump");
		}
	}

	m_acc = math::vec2f(0,0);

	if ( state.isPressed(ActionsClimber::Left) && !state.isPressed(ActionsClimber::Right))
	{
		setScaleWidth(-1); //Mirror del sprite
		if (grounded)
		{
			if (!crouched) m_acc -= run_acc;
		}
		else m_acc -= air_acc;
	}
	else if ( state.isPressed(ActionsClimber::Right) && !state.isPressed(ActionsClimber::Left))
	{
		setScaleWidth(+1); //Sprite sin mirror
		if (grounded)
		{
			if (!crouched) m_acc += run_acc;
		}
		else m_acc += air_acc;
	}

	//FRICTION
	m_fri = math::vec2f(0,0);
	if (grounded)
	{
		walljumped = false;
		if (crouched) m_fri.x = fri_acc.x/2;
		else
		{
			if (idle) m_fri.x = fri_acc.x;
			else      m_fri.x = fri_acc.x/4;
		}
	}
	else
	{
		if (ableToWallJump) m_fri.y = fri_acc.y;
		m_fri.x = fri_acc.x/8;
	}


	//Si en el frame anterior estaba tocando el suelo, inicializando
	//jumpTimeLeft a mas de 0 permite al jugador saltar durante ese rato
	
	if ( state.isDown(ActionsClimber::Jump) && (grounded||ableToWallJump))
	{
		s_sndHdl->stop();
		s_sndHdl->set_buffer(s_sndJump);
		s_sndHdl->play();

		jumpTimeLeft = jump_time;
		if (ableToWallJump) {
			walljumped = true;
			m_vel.x = vel_jmp.x * -1.0f * float(onWall);
		}
	}

	if (state.isPressed(ActionsClimber::Jump) && jumpTimeLeft > 0)
		m_vel.y = vel_jmp.y;
	else //A la que deja de pulsarse el boton de saltar cae de inmediato
		jumpTimeLeft = 0;

	if (jumpTimeLeft <= 0) m_acc += gra_acc; //La gravedad afecta mientras no salte
	else jumpTimeLeft -= deltaTime; //Se le resta el tiempo mientras salta

	if (m_lastAnim == "Walk") m_animVelFactor = sqrt(fabs(m_vel.x)/vel_max.x);
	else m_animVelFactor = 1.0f;

	if (state.isPressed(ActionsClimber::Special)) m_velLim = vel_max_run;
	else m_velLim = vel_max;
	TilemapCharacter::update(deltaTime);
}

void Player::noLeftCollision()
{
	onWall = ONWALL_NO;
}
void Player::noRightCollision()
{
	onWall = ONWALL_NO;
}

void Player::noUpCollision()
{
	grounded = false;
}

void Player::noDownCollision()
{
	grounded = false;
}


bool Player::onLeftCollision(int x, int j)
{
	/*
	if (!walljumped && fabs(m_vel.x) > 150)
	{
		m_vel.y = 0;
		onWall = ONWALL_LEFT;
	}
	*/
	m_vel.x = 0;
	return true;
}

bool Player::onRightCollision(int x, int j)
{
	/*
	if (!walljumped && fabs(m_vel.x) > 150)
	{
		m_vel.y = 0;
		onWall = ONWALL_RIGHT;
	}
	*/
	m_vel.x = 0;
	return true;
}

bool Player::onUpCollision(int x, int j)
{
	world.destroyBlock(x, j);
	s_sndHdl->stop();
	s_sndHdl->set_buffer(s_sndRoofHit);
	s_sndHdl->play();
	m_vel.y = 0;
	jumpTimeLeft = 0;
	grounded = false;
	return true;
}

bool Player::onDownCollision(int x, int j)
{
	m_vel.y = 0;
	grounded = true;
	return true;
}

void Player::reset()
{
	m_vel = math::vec2f(0,0);
}
