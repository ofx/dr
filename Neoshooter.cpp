#include "StdAfx.h"

#include "Neoshooter.h"

Neoshooter::Neoshooter(Player *owner, World *world) : Weapon(owner, world, "Neoshooter", "NS")
{
}

Neoshooter::~Neoshooter(void)
{
}

void Neoshooter::Shoot(void)
{
    cpSpaceAddPostStepCallback(this->m_World->GetSpace(), ShootP, this, this->m_Owner);
}

void Neoshooter::ShootP(cpSpace *space, void *obj, void *data)
{
    Neoshooter *instance = (Neoshooter*) obj;
    Player     *player   = (Player*) data;

    // Create 10 bullets
    for (int i = 0 ; i < 1 ; ++i)
    {
        NeoshooterBullet *bullet = new NeoshooterBullet(player);
        instance->m_World->AddGameObject(bullet);
    }
}