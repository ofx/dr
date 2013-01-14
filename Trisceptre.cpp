#include "StdAfx.h"

#include "Trisceptre.h"

Trisceptre::Trisceptre(Player *owner, World *world) : Weapon(owner, world, "Trisceptre", "TR")
{
}

Trisceptre::~Trisceptre(void)
{
}

void Trisceptre::Shoot(void)
{
    cpSpaceAddPostStepCallback(this->m_World->GetSpace(), ShootP, this, this->m_Owner);
}

void Trisceptre::ShootP(cpSpace *space, void *obj, void *data)
{
    Trisceptre *instance = (Trisceptre*) obj;
    Player     *player   = (Player*) data;

    // Create 10 bullets
    for (int i = 0 ; i < 1 ; ++i)
    {
        TrisceptreBullet *bullet = new TrisceptreBullet(player);
        instance->m_World->AddGameObject(bullet);
    }
}