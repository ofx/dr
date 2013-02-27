/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "GameObject.h"

GameObject::GameObject(void)
{
    this->m_Engine   = Engine::GetSingleton();
    this->m_Hge      = this->m_Engine->GetHge();
    this->m_Position = cpVect();
}

GameObject::~GameObject(void)
{
    
}
