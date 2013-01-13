/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "Weapon.h"

Weapon::Weapon(Player *owner, World *world, std::string name, std::string shortName) : WeaponName(name), WeaponShortName(shortName)
{
    this->m_Owner = owner;
    this->m_World = world;
}

Weapon::~Weapon(void)
{
}
