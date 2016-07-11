#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <direct.h>

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define GLM_FORCE_INLINE
#define GLM_FORCE_AVX
#include "glm/glm.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/quaternion.hpp"
#include "SFML/Audio.hpp"
#include "btBulletDynamicsCommon.h"
#include "lodepng.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "lua.hpp"
#include "pugixml.hpp"
#include "zlib.h"
#include "utf8.h"
