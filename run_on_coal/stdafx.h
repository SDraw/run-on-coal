#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
#include <unordered_map>
#include <list>
#include <atomic>
#include <ctime>
#include <direct.h>

#include "GL/glew.h"

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#define GLM_FORCE_INLINE
#ifndef _DEBUG
#define GLM_FORCE_SSE2
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"

#include "btBulletDynamicsCommon.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "MaxRectsBinPack.h"
#include "lua.hpp"
#include "pugixml.hpp"
#include "zlib.h"

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"
