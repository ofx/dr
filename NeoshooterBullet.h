#pragma once

#include "Bullet.h"

class NeoshooterBullet : public Bullet
{
private:
    HTEXTURE m_Texture;

    hgeSprite *m_Sprite;

    cpShape *m_Shape;
    cpBody  *m_Body;
public:
    NeoshooterBullet(Player *owner);
    ~NeoshooterBullet(void);

    void HandleDamage(Player *player);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);
};