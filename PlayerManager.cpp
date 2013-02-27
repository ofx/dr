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

float *PlayerManager::FindPlayerPositionCentroid(void) 
{
    float centroid[2] = {0.0f, 0.0f};

    std::list<Player*>::const_iterator it;
    for (it = this->m_Players->begin() ; it != this->m_Players->end() ; ++it)
    {
        cpVect pos = (*it)->GetPosition();
        centroid[0] += pos.x;
        centroid[1] += pos.y;
    }

    float iNumPlayers = 1.0f / this->m_Players->size();
    centroid[0] *= iNumPlayers;
    centroid[1] *= iNumPlayers;

    return centroid;
}

Player *PlayerManager::NewPlayer(void)
{
    static int i = 0;

    // Storage for the 
    cpVect pos;

    // Check if we have players, if so, place the new player at displaced player (first) location,
    // else, place the player at the default start location
    if (this->m_Players->size() > 0)
    {
        // Retrieve the first player
        Player *player = *this->m_Players->begin();

        cpVect playerPosition = player->GetPosition();
        pos.x = playerPosition.x;
        pos.y = playerPosition.y + PLAYER_SPAWN_OFFSET_Y;
    }
    else
    {
        pos.x = DEFAULT_PLAYER_SPAWN_X;
        pos.y = DEFAULT_PLAYER_SPAWN_Y;
    }

    // Retrieve the color list
    PLAYER_COLOR_SET;

    // Create the player
    Player *player = new Player(pos, PLAYER_COLOR_SET_NAME[i++], 0, "Player");
    this->m_Players->push_back(player);
    this->m_World->m_Objects.push_back(player);

    return player;
}

void PlayerManager::Initialize(void)
{
    /*cpVect pos;
    pos.x = 400.0f;
    pos.y = 600.0f;

    Player *player = new Player(pos, 0xFFFFA0FF, 0, "Player1");
    this->m_Players->push_back(player);
    this->m_World->m_Objects.push_back(player);*/
}

void PlayerManager::Render(float dt)
{
    // Follow the first player
    if (this->m_Players->size() > 0)
    {
        Player *firstPlayer = *this->m_Players->begin();
        cpVect firstPlayerPosition = firstPlayer->GetPosition();

        this->m_Engine->GetWorld()->WorldCamera->Position.x = (this->m_Engine->GetWidth() * 0.5f) - firstPlayerPosition.x;
        this->m_Engine->GetWorld()->WorldCamera->Position.y = (this->m_Engine->GetHeight() * 0.5f) - firstPlayerPosition.y;
    }
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
    this->m_Players->sort(PlayerSort());
}