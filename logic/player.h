#pragma once
#include "tilemapcharacter.h"

#include <memory>

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

    static std::shared_ptr<SpriteAnimData> s_data[2];

    //Mario-like player
    int m_player;
    bool m_crouched;
    bool m_grounded;
    bool m_walljumped;
    char m_onWall;
    float m_jumpTimeLeft;

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

    World &m_world;
};

