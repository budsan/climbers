#pragma once

#include "tilemapcharacter.h"
#include "guyframework/audio/emyl.h"

#include <boost/shared_ptr.hpp>

class World;
class Player : public TilemapCharacter
{
public:
    Player(World &world, int player);
    virtual void update(float deltaTime);

    static void load();
    static void unload();
    void reset();
private:

    static boost::shared_ptr<Guy::SpriteAnimData> s_sprData[2];
    static boost::shared_ptr<emyl::sound> s_sndHdl;
    static ALuint s_sndJump;
    static ALuint s_sndRoofHit;

    //Mario-like player
    int player;
    bool crouched;
    bool grounded;
    bool walljumped;
    char onWall;
    float jumpTimeLeft;

    enum {ONWALL_LEFT = -1, ONWALL_NO = 0, ONWALL_RIGHT = 1};
protected:

    virtual void noLeftCollision();
    virtual void noRightCollision();
    virtual void noUpCollision();
    virtual void noDownCollision();

    virtual bool onLeftCollision(int x, int j);
    virtual bool onRightCollision(int x, int j);
    virtual bool onUpCollision(int x, int j);
    virtual bool onDownCollision(int x, int j);

    World &world;
};

