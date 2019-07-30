#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

#include <Windows.h>

#define GLM_FORCE_MESSAGES
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "lua.hpp"
#include "pugixml.hpp"
#include "base64.h"

#include "Interfaces/IModule.h"
#include "Interfaces/ICore.h"
#include "Interfaces/IAsyncManager.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/ILogManager.h"
#include "Interfaces/INetworkManager.h"
#include "Interfaces/IPhysicsManager.h"
#include "Interfaces/IRenderManager.h"
#include "Interfaces/ISfmlManager.h"
#include "Interfaces/ISoundManager.h"
#include "Interfaces/IVRManager.h"
#include "Interfaces/IElement.h"
#include "Interfaces/ICollidable.h"
#include "Interfaces/IDrawable.h"
#include "Interfaces/IAnimation.h"
#include "Interfaces/ICamera.h"
#include "Interfaces/ICollision.h"
#include "Interfaces/IFile.h"
#include "Interfaces/IFont.h"
#include "Interfaces/IGeometry.h"
#include "Interfaces/ILight.h"
#include "Interfaces/IModel.h"
#include "Interfaces/IRenderTarget.h"
#include "Interfaces/IScene.h"
#include "Interfaces/ISound.h"
#include "Interfaces/ITexture.h"
#include "Interfaces/IShader.h"
#include "Interfaces/IShaderUniform.h"
