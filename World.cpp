/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

#include "World.h"

World::World(void)
{
    // Store a reference to the engine
    this->m_Engine = Engine::GetSingleton();

    // Create the camera
    this->WorldCamera = new Camera();
}

World::~World(void)
{
    // Remove all objects
    std::list<GameObject*>::const_iterator it;
    for (it = this->m_Objects.begin() ; it != this->m_Objects.end() ; ++it)
    {
        delete *it;
    }

    // Delete the camera
    delete this->WorldCamera;

    // Clear the list of objects
    this->m_Objects.clear();

    // Delete physics stuff
    cpSpaceFree(this->m_Space);
}

void World::RemoveGameObject(GameObject *object)
{
    this->m_DeletionList.push_back(object);
}

void World::InitializePhysics(void)
{
    // The space should have gravity
    cpVect gravity = cpv(0, -0.0001);
        
    // Create a physics space
    this->m_Space = cpSpaceNew();
    cpSpaceSetGravity(this->m_Space, gravity);
}

void World::Initialize(void)
{
    // Achtung: Watch the Z-order

    // Determine the grid size
    float gridSize = this->m_Engine->GetWidth() / 20;

    // Create the player manager
    this->m_PlayerManager = new PlayerManager();
    this->m_Objects.push_back(this->m_PlayerManager);

    // Create the grid manager
    this->m_LevelManager = new LevelGridManager();
    this->m_Objects.push_back(this->m_LevelManager);
}

void World::Render(float dt)
{
    // Call the render routines for all objects
    std::list<GameObject*>::const_iterator it;
    for (it = this->m_Objects.begin() ; it != this->m_Objects.end() ; ++it)
    {
        (*it)->Render(dt);
    }
}

void World::RenderText(hgeFont *font, float dt)
{
    // Call the rendertext routines for all objects
    std::list<GameObject*>::const_iterator it;
    for (it = this->m_Objects.begin() ; it != this->m_Objects.end() ; ++it)
    {
        (*it)->RenderText(font, dt);
    }
}

void World::Update(float dt)
{
    // Step through in physics world
    {
        cpFloat timeStep = 1.0 / 60.0;
        for (cpFloat time = 0; time < 2; time += timeStep) 
        {
            cpSpaceStep(this->m_Space, timeStep);
        }
    }

    // Call the update routines for all objects
    std::list<GameObject*>::const_iterator it;
    for (it = this->m_Objects.begin() ; it != this->m_Objects.end() ; ++it)
    {
        (*it)->Update(dt);
    }

    // Remove and delete
    for (it = this->m_DeletionList.begin() ; it != this->m_DeletionList.end() ; ++it)
    {
        GameObject *object = *it;
        this->m_Objects.remove(object);
        delete object;
    }
    this->m_DeletionList.clear();
}