/**
 * DroidRacers
 * by Marlon Etheredge
 */

#pragma once

// Die warnings
#pragma warning( disable : 4099 4244 4018 4966 4996 )

// WinSock
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")

#include "Debug.h"

// Stdlib
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vector>

// DirectX (need this to extend the functionality of HGE core a bit)
#include <d3d10.h>

// Hge
#include "hge.h"
#include "hgesprite.h"
#include "hgefont.h"
#include "hgeparticle.h"

// Chipmunk
#include "chipmunk/chipmunk.h"

// QREncode
#include "qrencode.h"

// External cJSON
#include "cJSON.h"

// DroidRacers
#include "GameObject.h"
#include "PhysicsObject.h"
#include "Bullet.h"
#include "NeoshooterBullet.h"
#include "Weapon.h"
#include "Neoshooter.h"
#include "Player.h"
#include "LevelGridManager.h"
#include "PlayerManager.h"
#include "World.h"
#include "DroidRacers.h"
#include "LevelGrid.h"
#include "ServerConnection.h"