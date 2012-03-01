#include "player.h"

#include "keys.h"

#include "log.h"
#include "input/input.h"
#include "world.h"

#include "audio/emyl.h"

std::shared_ptr<SpriteAnimData> Player::s_data[2] = {nullptr, nullptr};
ALuint jumpSound = 0;
ALuint roofhitSound = 0;
std::unique_ptr<emyl::sound> handler = nullptr;

Player::Player(World &world, int player) : TilemapCharacter(world.map), m_player(player), m_world(world)
{
    if (s_data[0] == nullptr) load();
    setAnimData(s_data[player]);
    ensureAnim("Idle");
    m_jumpTimeLeft = 0.0f;
    m_crouched = false;
    m_onWall = ONWALL_NO;
}

void Player::load()
{
    if (s_data[0] == nullptr)
    {
        s_data[0] = std::shared_ptr<SpriteAnimData>(new SpriteAnimData());
        if (!s_data[0]->load("data/scripts/mario01.anim"))
        {
            //LOG << "CRITICAL: data/scripts/mario01.anim doesn't exist." << std::endl;
        }
    }

    if (s_data[1] == nullptr)
    {
        s_data[1] = std::shared_ptr<SpriteAnimData>(new SpriteAnimData());
        if (!s_data[1]->load("data/scripts/mario02.anim"))
        {
            //LOG << "CRITICAL: data/scripts/mario02.anim doesn't exist." << std::endl;
        }
    }

    if (handler == nullptr)
    {
        handler = std::unique_ptr<emyl::sound>(new emyl::sound());
        handler->set_source();
    }

    emyl::manager* audiomng = emyl::manager::get_instance();
    if (jumpSound == 0) jumpSound = audiomng->get_buffer("data/sound/jump.ogg");
    if (roofhitSound == 0) roofhitSound = audiomng->get_buffer("data/sound/roofhit.ogg");
}

void Player::unload()
{
    s_data[0].reset();
    s_data[1].reset();
    handler.reset();
}

void Player::update(float deltaTime)
{
    InputState state = Input::instance().getInputState(m_player);

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
    m_crouched = ((m_crouched||m_grounded) && state.getKeyState(K_DOWN)) || (m_crouched&&!m_grounded);
    bool ableToWallJump = m_onWall && !m_grounded && !m_crouched&& !m_walljumped;

    if (m_crouched)
    {
        ensureAnim("Crouch");
    }
    else
    {
        if (m_grounded)
        {
            if (state.getKeyState(K_LEFT) && !state.getKeyState(K_RIGHT))
            {
                if (m_vel.x > 0) ensureAnim("Turn");
                else ensureAnim("Walk");
            }
            else if (state.getKeyState(K_RIGHT) && !state.getKeyState(K_LEFT))
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

    if ( state.getKeyState(K_LEFT) && !state.getKeyState(K_RIGHT))
    {
        setScaleWidth(-1); //Mirror del sprite
        if (m_grounded)
        {
            if (!m_crouched) m_acc -= run_acc;
        }
        else m_acc -= air_acc;
    }
    else if ( state.getKeyState(K_RIGHT) && !state.getKeyState(K_LEFT))
    {
        setScaleWidth(+1); //Sprite sin mirror
        if (m_grounded)
        {
            if (!m_crouched) m_acc += run_acc;
        }
        else m_acc += air_acc;
    }

    //FRICTION
    m_fri = math::vec2f(0,0);
    if (m_grounded)
    {
        m_walljumped = false;
        if (m_crouched) m_fri.x = fri_acc.x/2;
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

    if ( state.getKeyDown(K_JUMP) && (m_grounded||ableToWallJump))
    {
        handler->play_buffer(jumpSound);

        m_jumpTimeLeft = jump_time;
        if (ableToWallJump) {
            m_walljumped = true;
            m_vel.x = vel_jmp.x * -1.0f * float(m_onWall);
        }
    }

    if (state.getKeyState(K_JUMP) && m_jumpTimeLeft > 0)
        m_vel.y = vel_jmp.y;
    else //A la que deja de pulsarse el boton de saltar cae de inmediato
        m_jumpTimeLeft = 0;

    if (m_jumpTimeLeft <= 0) m_acc += gra_acc; //La gravedad afecta mientras no salte
    else m_jumpTimeLeft -= deltaTime; //Se le resta el tiempo mientras salta

    if (m_lastAnim == "Walk") m_animVelFactor = sqrt(fabs(m_vel.x)/vel_max.x);
    else m_animVelFactor = 1.0f;

    if (state.getKeyState(K_SPECIAL)) m_velLim = vel_max_run;
    else m_velLim = vel_max;
    TilemapCharacter::update(deltaTime);
}

void Player::noLeftCollision()
{
    m_onWall = ONWALL_NO;
}
void Player::noRightCollision()
{
    m_onWall = ONWALL_NO;
}

void Player::noUpCollision()
{
    m_grounded = false;
}

void Player::noDownCollision()
{
    m_grounded = false;
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
    m_vel.x = 0;
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
    m_vel.x = 0;
    return true;
}

bool Player::onUpCollision(int x, int j)
{
    m_world.destroyBlock(x, j);
    handler->play_buffer(roofhitSound);
    m_vel.y = 0;
    m_jumpTimeLeft = 0;
    m_grounded = false;
    return true;
}

bool Player::onDownCollision(int x, int j)
{
    m_vel.y = 0;
    m_grounded = true;
    return true;
}

void Player::reset()
{
    m_vel = math::vec2f(0,0);
}
