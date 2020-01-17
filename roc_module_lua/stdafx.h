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

#include "client/IModule.h"
#include "client/ICore.h"
#include "client/IAsyncManager.h"
#include "client/IElementManager.h"
#include "client/ILogManager.h"
#include "client/INetworkManager.h"
#include "client/IPhysicsManager.h"
#include "client/IRenderManager.h"
#include "client/ISfmlManager.h"
#include "client/ISoundManager.h"
#include "client/IVRManager.h"
#include "client/IElement.h"
#include "client/ICollidable.h"
#include "client/IDrawable.h"
#include "client/IAnimation.h"
#include "client/ICamera.h"
#include "client/ICollision.h"
#include "client/IFile.h"
#include "client/IFont.h"
#include "client/IGeometry.h"
#include "client/ILight.h"
#include "client/IModel.h"
#include "client/IRenderTarget.h"
#include "client/IScene.h"
#include "client/IShader.h"
#include "client/IShaderUniform.h"
#include "client/ISound.h"
#include "client/ITexture.h"
#include "client/ICustomArgument.h"
#include "client/ICustomArguments.h"

#include "lua.hpp"
#include "pugixml.hpp"
#include "base64.h"
