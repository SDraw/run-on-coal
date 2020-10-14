#pragma once
#pragma warning (disable : 4250)
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <atomic>
#include <thread>
#include <mutex>
#include <ctime>

#include <Windows.h>

#include "GL/glew.h"

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
#include "SFML/GpuPreference.hpp"

#include "openvr.h"

#include "glm/glm.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "btBulletDynamicsCommon.h"

#include "ft2build.h"
#include FT_FREETYPE_H
#include "MaxRectsBinPack.h"
#include "pugixml.hpp"
#include "zlib.h"

#ifdef _DEBUG
#define DEBUG_PRINT(T) std::cout << T << std::endl
#endif
