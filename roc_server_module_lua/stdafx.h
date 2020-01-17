#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <limits>

#include <Windows.h>

#include "server/IModule.h"
#include "server/ICore.h"
#include "server/IElementManager.h"
#include "server/ILogManager.h"
#include "server/INetworkManager.h"
#include "server/IElement.h"
#include "server/IClient.h"
#include "server/IFile.h"
#include "server/ICustomArgument.h"
#include "server/ICustomArguments.h"

#include "lua.hpp"
#include "pugixml.hpp"
#include "base64.h"
