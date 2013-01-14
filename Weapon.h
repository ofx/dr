/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <string>

#define NUM_WEAPONS             1

#define WEAPON_INDEX_NEOSHOOTER 0

class World;

class Weapon
{
protected:
    Player *m_Owner;

    World *m_World;
public:
    const std::string WeaponName;
    const std::string WeaponShortName;

    Weapon(Player *owner, World *world, std::string name, std::string shortName);
    virtual ~Weapon(void);

    virtual void Shoot(void) = 0;
};