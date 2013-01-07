/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "StdAfx.h"

// Callback wrappers
bool GfxRestoreFuncWrapper(void)
{
    return Engine::GetSingleton()->GfxRestoreFunc();
}
bool FrameFuncWrapper(void)
{
    return Engine::GetSingleton()->FrameFunc();
}
bool RenderFuncWrapper(void)
{
    return Engine::GetSingleton()->RenderFunc();
}

Engine *Engine::s_Singleton = 0;

Engine::Engine(void)
{
    // Not yet booted
    this->m_Booted = false;
    
    // Not yet shutdown
    this->m_IsShutdown = false;

    // Create the HGE instance
    this->m_Hge = hgeCreate(HGE_VERSION);

    // Set callbacks
	this->m_Hge->System_SetState(HGE_FRAMEFUNC,      FrameFuncWrapper);
	this->m_Hge->System_SetState(HGE_RENDERFUNC,     RenderFuncWrapper);
	this->m_Hge->System_SetState(HGE_GFXRESTOREFUNC, GfxRestoreFuncWrapper);
}

Engine::~Engine(void)
{
    // Delete the world
    delete this->m_World;
}

Engine *Engine::GetSingleton(void)
{
    return Engine::s_Singleton == 0 ? Engine::s_Singleton = new Engine() : Engine::s_Singleton;
}

HGE *Engine::GetHge(void)
{
    return this->m_Hge;
}

void Engine::SetDrawStats(bool stats)
{
    this->m_DrawStats = stats;
}

void Engine::SetLogFile(std::string logFile)
{
    if (!this->m_Booted)
    {
        this->m_LogFile = logFile;
    }
}

void Engine::SetTitle(std::string title)
{
    if (!this->m_Booted)
    {
        this->m_Title = title;
    }
}

void Engine::SetWindowed(bool windowed)
{
    if (!this->m_Booted)
    {
        this->m_Windowed = windowed;
    }
}

void Engine::SetFps(unsigned int fps)
{
    if (!this->m_Booted)
    {
        this->m_Fps = fps;
    }
}

void Engine::SetWidth(unsigned int width)
{
    if (!this->m_Booted)
    {
        this->m_Width = width;
    }
}

void Engine::SetHeight(unsigned int height)
{
    if (!this->m_Booted)
    {
        this->m_Height = height;
    }
}

void Engine::SetBpp(unsigned int bpp)
{
    if (!this->m_Booted)
    {
        this->m_Bpp = bpp;
    }
}

bool Engine::FrameFunc(void)
{
    // In case of shutdown state, halt
    if (this->m_IsShutdown)
    {
        return true;
    }

    // Get delta time
    float dt = this->m_Hge->Timer_GetDelta();

    // Update the world
    this->m_World->Update(dt);

    return false;
}

bool Engine::GfxRestoreFunc(void)
{
    // In case of shutdown state, halt
    if (this->m_IsShutdown)
    {
        return true;
    }

    return false;
}

bool Engine::RenderFunc(void)
{
    // In case of shutdown state, halt
    if (this->m_IsShutdown)
    {
        return true;
    }

    // Get delta time
    float dt = this->m_Hge->Timer_GetDelta();

    // Begin and clear scene
    this->m_Hge->Gfx_Clear(0);
    this->m_Hge->Gfx_BeginScene();

        // Transpose along camera coordinates
        this->m_Hge->Gfx_SetTransform(0, 0, this->m_World->WorldCamera->Position.x, this->m_World->WorldCamera->Position.y, 0, 1, 1);
        {
            // Let the world render
            this->m_World->Render(dt);
        }
        
        // Reset the camera
        this->m_Hge->Gfx_SetTransform();

        // If requested, draw some stats
	    if (this->m_DrawStats)
        {
	        this->m_DefaultFont->printf(5, this->GetHeight() - 40.0f, HGETEXT_LEFT, "dt:%.3f\nFPS:%d", this->m_Hge->Timer_GetDelta(), this->m_Hge->Timer_GetFPS());
        }

        // Pop some font data
        float scale = this->m_DefaultFont->GetScale();
        DWORD color = this->m_DefaultFont->GetColor();

        // Give the world an opportunity to render some text
        this->m_World->RenderText(this->m_DefaultFont, dt);

        // Reset the font data
        this->m_DefaultFont->SetScale(scale);
        this->m_DefaultFont->SetColor(color);

    // End of scene
    this->m_Hge->Gfx_EndScene();

	return false;
}

void Engine::DrawLine(hgeVector p1, hgeVector p2, DWORD color, float width, int blend)
{
	hgeQuad q;
	hgeVector normal = (p2 - p1);
	normal.Normalize();
	normal *= width * 0.5f;
	normal.Rotate(M_PI * 0.5f);
 
	q.tex = NULL;
	q.blend = blend;
	q.v[0].x = (p1 + normal).x; q.v[0].y = (p1 + normal).y; q.v[0].z = 0.5f; q.v[0].col = color;
	q.v[1].x = (p1 - normal).x; q.v[1].y = (p1 - normal).y; q.v[1].z = 0.5f; q.v[1].col = color;
	q.v[2].x = (p2 - normal).x; q.v[2].y = (p2 - normal).y; q.v[2].z = 0.5f; q.v[2].col = color;
	q.v[3].x = (p2 + normal).x; q.v[3].y = (p2 + normal).y; q.v[3].z = 0.5f; q.v[3].col = color;
 
	this->m_Hge->Gfx_RenderQuad(&q);
}

void Engine::Shutdown(void)
{
    // Clean up and shutdown
	this->m_Hge->System_Shutdown();
	this->m_Hge->Release();

    // Indicate that we've shutdown
    this->m_IsShutdown = true;
}

void Engine::Boot(void)
{
    // Set all system states
	this->m_Hge->System_SetState(HGE_LOGFILE,        this->m_LogFile.c_str());
	this->m_Hge->System_SetState(HGE_TITLE,          this->m_Title.c_str());
	this->m_Hge->System_SetState(HGE_FPS,            this->m_Fps);
	this->m_Hge->System_SetState(HGE_WINDOWED,       this->m_Windowed);
	this->m_Hge->System_SetState(HGE_SCREENWIDTH,    this->m_Width);
	this->m_Hge->System_SetState(HGE_SCREENHEIGHT,   this->m_Height);
	this->m_Hge->System_SetState(HGE_SCREENBPP,      this->m_Bpp);

    // Initiate the system
    this->m_Hge->System_Initiate();

    // Load the default font
    this->m_DefaultFont = new hgeFont("data/DefaultFont.fnt");
    this->m_DefaultFont->SetScale(0.75f);
    this->m_DefaultFont->SetColor(0xFFE590E5);

    // Load the music stream
    this->m_MainMusic = this->m_Hge->Stream_Load("data/harmonic.mp3");

    // Player music
    this->m_Hge->Stream_Play(this->m_MainMusic, true);

    // Create the world
    this->m_World = new World();

    // Normally I would want to auto call initialize routines, but due to the
    // fact that subs of the world need a fully constructed world, do so first,
    // then call initialize (physics first)
    this->m_World->InitializePhysics();

    // Initialize the world
    this->m_World->Initialize();

    // Systems go
    this->m_Hge->System_Start();

    // Indicate that we're booted
    this->m_Booted = true;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Load, initialize, whatever
    Engine *engine = Engine::GetSingleton();
    engine->SetLogFile("DroidRacers.log");
	engine->SetTitle("DroidRacers");
	engine->SetFps(100);
	engine->SetWindowed(true);
	engine->SetWidth(800);
	engine->SetHeight(600);
	engine->SetBpp(32);

	// Boot
    engine->Boot();

    // Delete the engine, bye
    delete engine;

	return 0;
}
