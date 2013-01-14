#pragma once

#include "Bullet.h"

class TrisceptreBullet : public Bullet
{
private:
    HTEXTURE m_Texture;

    hgeSprite *m_Sprite;

    cpShape *m_Shape;
    cpBody  *m_Body;
public:
    TrisceptreBullet(Player *owner);
    ~TrisceptreBullet(void);

    void HandleDamage(Player *player);

    void Initialize(void);
    void Render(float dt);
    void Update(float dt);
};