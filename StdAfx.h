#pragma once

// Die warnings
#pragma warning( disable : 4099 4244 4018 4966 )

#include "Debug.h"

// Stdlib
#include <string>

// DirectX (need this to extend the functionality of HGE core a bit)
#include <d3d10.h>

// Hge
/**
 * DroidRacers
 * by Marlon Etheredge
 */

#include "hge.h"
#include "hgesprite.h"
#include "hgefont.h"
#include "hgeparticle.h"

// Chipmunk
#include "chipmunk/chipmunk.h"

// DroidRacers
#include "GameObject.h"
#include "Player.h"
#include "LevelGridManager.h"
#include "PlayerManager.h"
#include "World.h"
#include "DroidRacers.h"
#include "LevelGrid.h"
#include "Level.h"