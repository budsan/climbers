#include "player.h"

#include "log.h"
#include "input/input.h"
#include "world.h"

#include "audio/emyl.h"

SpriteAnimData *Player::data[2] = {NULL, NULL};
ALuint jumpSound = 0;
ALuint roofhitSound = 0;
emyl::sound *handler = NULL;

Player::Player(World &world, int player) : TilemapCharacter(world.map), player(player), world(world)
{
	if (data[0] == NULL) Load();
	setAnimData(data[player]);
	ensureAnim("Idle");
	jumpTimeLeft = 0.0f;
	crouched = false;
	onWall = ONWALL_NO;
}

void Player::Load()
{
	if (data[0] == NULL)
	{
		data[0] = new SpriteAnimData();
		if (!data[0]->Load("data/scripts/mario01.anim"))
		{
			//LOG << "CRITICAL: data/scripts/mario01.anim doesn't exist." << std::endl;
		}
	}

	if (data[1] == NULL)
	{
		data[1] = new SpriteAnimData();
		if (!data[1]->Load("data/scripts/mario02.anim"))
		{
			//LOG << "CRITICAL: data/scripts/mario02.anim doesn't exist." << std::endl;
		}
	}

	if (handler == NULL)
	{
		handler = new emyl::sound();
		handler->set_source();
	}

	emyl::manager* audiomng = emyl::manager::get_instance();
	if (jumpSound == 0) jumpSound = audiomng->get_buffer("data/sound/jump.ogg");
	if (roofhitSound == 0) roofhitSound = audiomng->get_buffer("data/sound/roofhit.ogg");
}

void Player::Unload()
{
	if (data[0] != NULL)
	{
		delete data[0];
		data[0] = NULL;
	}

	if (data[1] != NULL)
	{
		delete data[1];
		data[1] = NULL;
	}

	if (handler != NULL)
	{
		delete handler;
		handler = NULL;
	}
}

void Player::Update(float GameTime)
{
	InputState state = Input::Instance().getInputState(player);

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
	crouched = ((crouched||grounded) && state.getKeyState(K_DOWN)) || (crouched&&!grounded);
	bool ableToWallJump = onWall && !grounded && !crouched&& !walljumped;

	if (crouched)
	{
		ensureAnim("Crouch");
	}
	else
	{
		if (grounded)
		{
			if (state.getKeyState(K_LEFT) && !state.getKeyState(K_RIGHT))
			{
				if (vel.x > 0) ensureAnim("Turn");
				else ensureAnim("Walk");
			}
			else if (state.getKeyState(K_RIGHT) && !state.getKeyState(K_LEFT))
			{
				if (vel.x < 0) ensureAnim("Turn");
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

	acc = math::vec2f(0,0);

	if ( state.getKeyState(K_LEFT) && !state.getKeyState(K_RIGHT))
	{
		setScaleWidth(-1); //Mirror del sprite
		if (grounded)
		{
			if (!crouched) acc -= run_acc;
		}
		else acc -= air_acc;
	}
	else if ( state.getKeyState(K_RIGHT) && !state.getKeyState(K_LEFT))
	{
		setScaleWidth(+1); //Sprite sin mirror
		if (grounded)
		{
			if (!crouched) acc += run_acc;
		}
		else acc += air_acc;
	}

	//FRICTION
	fri = math::vec2f(0,0);
	if (grounded)
	{
		walljumped = false;
		if (crouched) fri.x = fri_acc.x/2;
		else
		{
			if (idle) fri.x = fri_acc.x;
			else      fri.x = fri_acc.x/4;
		}
	}
	else
	{
		if (ableToWallJump) fri.y = fri_acc.y;
		fri.x = fri_acc.x/8;
	}


	//Si en el frame anterior estaba tocando el suelo, inicializando
	//jumpTimeLeft a mas de 0 permite al jugador saltar durante ese rato
	
	if ( state.getKeyDown(K_JUMP) && (grounded||ableToWallJump))
	{
		handler->stop();
		handler->set_buffer(jumpSound);
		handler->play();

		jumpTimeLeft = jump_time;
		if (ableToWallJump) {
			walljumped = true;
			vel.x = vel_jmp.x * -1.0f * float(onWall);
		}
	}

	if (state.getKeyState(K_JUMP) && jumpTimeLeft > 0)
		vel.y = vel_jmp.y;
	else //A la que deja de pulsarse el boton de saltar cae de inmediato
		jumpTimeLeft = 0;

	if (jumpTimeLeft <= 0) acc += gra_acc; //La gravedad afecta mientras no salte
	else jumpTimeLeft -= GameTime; //Se le resta el tiempo mientras salta

	if (lastAnim == "Walk") animVelFactor = sqrt(fabs(vel.x)/vel_max.x);
	else animVelFactor = 1.0f;

	if (state.getKeyState(K_SPECIAL)) vel_lim = vel_max_run;
	else vel_lim = vel_max;
	TilemapCharacter::Update(GameTime);
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
	if (!walljumped && fabs(vel.x) > 150)
	{
		vel.y = 0;
		onWall = ONWALL_LEFT;
	}
	*/
	vel.x = 0;
	return true;
}

bool Player::onRightCollision(int x, int j)
{
	/*
	if (!walljumped && fabs(vel.x) > 150)
	{
		vel.y = 0;
		onWall = ONWALL_RIGHT;
	}
	*/
	vel.x = 0;
	return true;
}

bool Player::onUpCollision(int x, int j)
{
	world.destroyBlock(x, j);
	handler->stop();
	handler->set_buffer(roofhitSound);
	handler->play();
	vel.y = 0;
	jumpTimeLeft = 0;
	grounded = false;
	return true;
}

bool Player::onDownCollision(int x, int j)
{
	vel.y = 0;
	grounded = true;
	return true;
}

void Player::Reset()
{
	vel = math::vec2f(0,0);
}
