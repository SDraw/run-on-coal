#pragma once
#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#elif __linux__
#include <unistd.h>
#include <sys/time.h>
#define _getcwd getcwd
#endif

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>
#include <fstream>
#include <sstream>
#include <ctime>
#include <regex>

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
#include "lua.hpp"
#include "pugixml.hpp"
#include "zlib.h"
