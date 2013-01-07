/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

Level::Level(void)
{
    // Initialize the level
    this->Initialize();
}

Level::~Level(void)
{
}

void Level::Initialize(void)
{
    // Build the initial level
    for (int i = 0 ; i < 100 ; ++i)
    {
        cpVect *left = new cpVect();
        cpVect *right = new cpVect();

        left->x = 100.0f;     right->x = 200.0f;
        left->y = i * 100.0f; right->y = i * 100.0f;

        this->m_LeftVertexList.push_back(left);
        this->m_RightVertexList.push_back(right);
    }
}

void Level::Render(float dt)
{
    // Render all vertices
    std::list<cpVect*>::const_iterator it;
    std::list<cpVect*>::const_iterator nend = this->m_LeftVertexList.end(); std::advance(nend, -1);
    for (it = this->m_LeftVertexList.begin() ; it != nend ; ++it)
    {
        this->m_Hge->Gfx_RenderLine(
            (*it)->x,
            (*it)->y,
            (*(++it))->x,
            (*it)->y
        );

        --it;
    }
    nend = this->m_RightVertexList.end(); std::advance(nend, -1);
    for (it = this->m_RightVertexList.begin() ; it != nend ; ++it)
    {
        this->m_Hge->Gfx_RenderLine(
            (*it)->x,
            (*it)->y,
            (*(++it))->x,
            (*it)->y
        );

        --it;
    }
}

void Level::Update(float dt)
{

}