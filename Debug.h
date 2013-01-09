/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

#ifdef _DEBUG
//#define RENDER_BOUNDARIES
#endif

// For memory leak detection
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

class Debug
{
public:
    static void InitDebug(void);
};