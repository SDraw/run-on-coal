#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
#include <set>
#include <thread>
#include <atomic>
#include <ctime>
#ifndef _WIN32
#include <csignal>
#endif
#include <direct.h>

#include "SFML/System.hpp"
#include "lua.hpp"
#include "pugixml.hpp"

#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include "PacketLogger.h"
