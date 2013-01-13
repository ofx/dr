/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "PlayerManager.h"

PlayerManager::PlayerManager(void)
{
    this->m_World = this->m_Engine->GetWorld();

    this->m_Players = new std::list<Player*>();

    // Initialize
    this->Initialize();
}

PlayerManager::~PlayerManager(void)
{
    // Delete the player list
    delete this->m_Players;
}

void PlayerManager::Initialize(void)
{
    cpVect pos;
    pos.x = 400.0f;
    pos.y = 600.0f;

    Player *player = new Player(pos, 0xFFFFA0FF, 0, "Player2");
    this->m_Players->push_back(player);
    this->m_World->m_Objects.push_back(player);

    /*pos.x += 20.0f;

    player = new Player(pos, 0xFF6CCCBE, 1, "Player1");
    this->m_Players->push_back(player);
    this->m_World->m_Objects.push_back(player);*/
}

void PlayerManager::Render(float dt)
{
}

void PlayerManager::RenderText(hgeFont *font, float dt)
{
    // Display some player info
    font->printf(
        5, 5, 
        HGETEXT_LEFT, 
        "Players connected: %i\n--------------------",
        this->m_Players->size()
    );

    // Display the bottom line
    font->printf(
        5, 5 + (this->m_Players->size() + 2) * 20,
        HGETEXT_LEFT,
        "--------------------"
    );

    // Display all player names
    int i = 2;
    std::list<Player*>::const_iterator it;
    for (it = this->m_Players->begin() ; it != this->m_Players->end() ; ++it)
    {
        // Set the font to the player color
        font->SetColor((*it)->GetColor());

        // Construct a string of all weapon shortcuts
        std::string weaponString("");
        for (int i = 0 ; i < NUM_WEAPON_SLOTS ; ++i)
        {
            if ((*it)->m_Weaponslots[i])
            {
                bool active = (*it)->m_ActiveWeaponslot == i;
                weaponString += active ? "[" : "";
                weaponString += (*it)->m_Weaponslots[i]->WeaponShortName;
                weaponString += active ? "]" : "";
                weaponString += ",";
            }
        }
        weaponString = weaponString.substr(0, weaponString.size() - 1);

        // Print the player's name
        font->printf(
            5, 5 + 20 * i++, 
            HGETEXT_LEFT, 
            "%s (%s)",
            (*it)->GetName().c_str(),
            weaponString.c_str()
        );
    }
}

void PlayerManager::Update(float dt)
{
    // Determine the ranking
    this->m_Players->sort(std::greater<Player*>());
}