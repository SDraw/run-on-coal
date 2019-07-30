#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <Windows.h>

#include "lua.hpp"
#include "pugixml.hpp"
#include "base64.h"

#include "Interfaces/IModule.h"
#include "Interfaces/ICore.h"
#include "Interfaces/IElementManager.h"
#include "Interfaces/ILogManager.h"
#include "Interfaces/INetworkManager.h"
#include "Interfaces/IElement.h"
#include "Interfaces/IFile.h"
#include "Interfaces/IClient.h"
