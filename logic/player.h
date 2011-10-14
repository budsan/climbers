#pragma once
#include "tilemapcharacter.h"

class World;
class Player : public TilemapCharacter
{
public:
    Player(World &world, int player);
    virtual void Update(float GameTime);

    static void Load();
    static void Unload();
    void Reset();
private:

    static SpriteAnimData *data[2];

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

