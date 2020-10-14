#pragma once

#define WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

#include <Windows.h>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "IModule.h"
#include "ICore.h"
#include "IAsyncManager.h"
#include "IElementManager.h"
#include "ILogManager.h"
#include "IPhysicsManager.h"
#include "IRenderManager.h"
#include "ISfmlManager.h"
#include "ISoundManager.h"
#include "IVRManager.h"
#include "IElement.h"
#include "ICollidable.h"
#include "IDrawable.h"
#include "IAnimation.h"
#include "ICamera.h"
#include "ICollision.h"
#include "IFont.h"
#include "IGeometry.h"
#include "ILight.h"
#include "IModel.h"
#include "IRenderTarget.h"
#include "IScene.h"
#include "IShader.h"
#include "ISound.h"
#include "ITexture.h"
#include "ICustomArgument.h"
#include "ICustomArguments.h"

#include "lua.hpp"
#include "pugixml.hpp"
#include "base64.h"
