/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#include <string>

class Weapon
{
public:
    const std::string WeaponName;
    const std::string WeaponShortName;

    Weapon(std::string name, std::string shortName);
    virtual ~Weapon(void);

    virtual void Shoot(void) = 0;
};