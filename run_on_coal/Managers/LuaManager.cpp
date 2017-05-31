#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaDefinitions.Animation.h"
#include "Lua/LuaDefinitions.Camera.h"
#include "Lua/LuaDefinitions.Collision.h"
#include "Lua/LuaDefinitions.Drawable.h"
#include "Lua/LuaDefinitions.Element.h"
#include "Lua/LuaDefinitions.Events.h"
#include "Lua/LuaDefinitions.Font.h"
#include "Lua/LuaDefinitions.Light.h"
#include "Lua/LuaDefinitions.Geometry.h"
#include "Lua/LuaDefinitions.Model.h"
#include "Lua/LuaDefinitions.Movie.h"
#include "Lua/LuaDefinitions.Network.h"
#include "Lua/LuaDefinitions.Physics.h"
#include "Lua/LuaDefinitions.Rendering.h"
#include "Lua/LuaDefinitions.RenderTarget.h"
#include "Lua/LuaDefinitions.Scene.h"
#include "Lua/LuaDefinitions.Input.h"
#include "Lua/LuaDefinitions.Shader.h"
#include "Lua/LuaDefinitions.Sound.h"
#include "Lua/LuaDefinitions.Texture.h"
#include "Lua/LuaDefinitions.File.h"
#include "Lua/LuaDefinitions.Utils.h"

ROC::Core* ROC::LuaManager::m_coreStatic = NULL;

ROC::LuaManager::LuaManager(Core *f_core)
{
    m_core = f_core;
    m_coreStatic = f_core;

    m_vm = luaL_newstate();
    luaL_requiref(m_vm, "_G", luaopen_base, 1);
    luaL_requiref(m_vm, "math", luaopen_math, 1);
    luaL_requiref(m_vm, "string", luaopen_string, 1);
    luaL_requiref(m_vm, "table", luaopen_table, 1);
    luaL_requiref(m_vm, "bit32", luaopen_bit32, 1);
    luaL_requiref(m_vm, "utf8", luaopen_utf8, 1);

    //Security?
    lua_register(m_vm, "dofile", Lua::disabledFunction);
    lua_register(m_vm, "loadfile", Lua::disabledFunction);

    //Log
    lua_register(m_vm, "logPrint", Lua::logPrint);

    //Geometry
    lua_register(m_vm, "geometryCreate", Lua::geometryCreate);

    //Model
    lua_register(m_vm, "modelCreate", Lua::modelCreate);
    lua_register(m_vm, "modelGetGeometry", Lua::modelGetGeometry);
    lua_register(m_vm, "modelSetPosition", Lua::modelSetPosition);
    lua_register(m_vm, "modelGetPosition", Lua::modelGetPosition);
    lua_register(m_vm, "modelSetRotation", Lua::modelSetRotation);
    lua_register(m_vm, "modelGetRotation", Lua::modelGetRotation);
    lua_register(m_vm, "modelSetScale", Lua::modelSetScale);
    lua_register(m_vm, "modelGetScale", Lua::modelGetScale);
    lua_register(m_vm, "modelDraw", Lua::modelDraw);
    lua_register(m_vm, "modelAttach", Lua::modelAttach);
    lua_register(m_vm, "modelDetach", Lua::modelDetach);
    lua_register(m_vm, "modelGetParent", Lua::modelGetParent);
    lua_register(m_vm, "modelSetAnimation", Lua::modelSetAnimation);
    lua_register(m_vm, "modelGetAnimation", Lua::modelGetAnimation);
    lua_register(m_vm, "modelPlayAnimation", Lua::modelPlayAnimation);
    lua_register(m_vm, "modelPauseAnimation", Lua::modelPauseAnimation);
    lua_register(m_vm, "modelResetAnimation", Lua::modelResetAnimation);
    lua_register(m_vm, "modelSetAnimationProperty", Lua::modelSetAnimationProperty);
    lua_register(m_vm, "modelGetAnimationProperty", Lua::modelGetAnimationProperty);
    lua_register(m_vm, "modelGetCollision", Lua::modelGetCollision);

    //Shader
    lua_register(m_vm, "shaderCreate", Lua::shaderCreate);
    lua_register(m_vm, "shaderSetUniformValue", Lua::shaderSetUniformValue);

    //Scene
    lua_register(m_vm, "sceneCreate", Lua::sceneCreate);
    lua_register(m_vm, "sceneSetCamera", Lua::sceneSetCamera);
    lua_register(m_vm, "sceneGetCamera", Lua::sceneGetCamera);
    lua_register(m_vm, "sceneSetLight", Lua::sceneSetLight);
    lua_register(m_vm, "sceneGetLight", Lua::sceneGetLight);

    //Camera
    lua_register(m_vm, "cameraCreate", Lua::cameraCreate);
    lua_register(m_vm, "cameraSetPosition", Lua::cameraSetPosition);
    lua_register(m_vm, "cameraGetPosition", Lua::cameraGetPosition);
    lua_register(m_vm, "cameraSetDirection", Lua::cameraSetDirection);
    lua_register(m_vm, "cameraGetDirection", Lua::cameraGetDirection);
    lua_register(m_vm, "cameraSetType", Lua::cameraSetType);
    lua_register(m_vm, "cameraGetType", Lua::cameraGetType);
    lua_register(m_vm, "cameraSetFOV", Lua::cameraSetFOV);
    lua_register(m_vm, "cameraGetFOV", Lua::cameraGetFOV);
    lua_register(m_vm, "cameraSetAspectRatio", Lua::cameraSetAspectRatio);
    lua_register(m_vm, "cameraGetAspectRatio", Lua::cameraGetAspectRatio);
    lua_register(m_vm, "cameraSetOrthoParams", Lua::cameraSetOrthoParams);
    lua_register(m_vm, "cameraGetOrthoParams", Lua::cameraGetOrthoParams);
    lua_register(m_vm, "cameraSetDepth", Lua::cameraSetDepth);
    lua_register(m_vm, "cameraGetDepth", Lua::cameraGetDepth);
    lua_register(m_vm, "cameraGetViewMatrix", Lua::cameraGetViewMatrix);
    lua_register(m_vm, "cameraGetProjectionMatrix", Lua::cameraGetProjectionMatrix);

    //Light
    lua_register(m_vm, "lightCreate", Lua::lightCreate);
    lua_register(m_vm, "lightSetParams", Lua::lightSetParams);
    lua_register(m_vm, "lightGetParams", Lua::lightGetParams);
    lua_register(m_vm, "lightSetColor", Lua::lightSetColor);
    lua_register(m_vm, "lightGetColor", Lua::lightGetColor);
    lua_register(m_vm, "lightSetDirection", Lua::lightSetDirection);
    lua_register(m_vm, "lightGetDirection", Lua::lightGetDirection);

    //Animation
    lua_register(m_vm, "animationCreate", Lua::animationCreate);

    //Sound
    lua_register(m_vm, "soundCreate", Lua::soundCreate);
    lua_register(m_vm, "soundPlay", Lua::soundPlay);
    lua_register(m_vm, "soundPause", Lua::soundPause);
    lua_register(m_vm, "soundStop", Lua::soundStop);
    lua_register(m_vm, "soundIsLooped", Lua::soundIsLooped);
    lua_register(m_vm, "soundGetState", Lua::soundGetState);
    lua_register(m_vm, "soundSetSpeed", Lua::soundSetSpeed);
    lua_register(m_vm, "soundGetSpeed", Lua::soundGetSpeed);
    lua_register(m_vm, "soundSetVolume", Lua::soundSetVolume);
    lua_register(m_vm, "soundGetVolume", Lua::soundGetVolume);
    lua_register(m_vm, "soundSetTime", Lua::soundSetTime);
    lua_register(m_vm, "soundGetTime", Lua::soundGetTime);
    lua_register(m_vm, "soundGetDuration", Lua::soundGetDuration);
    lua_register(m_vm, "soundSet3DEnabled", Lua::soundSet3DEnabled);
    lua_register(m_vm, "soundGet3DEnabled", Lua::soundGet3DEnabled);
    lua_register(m_vm, "soundSet3DPosition", Lua::soundSet3DPosition);
    lua_register(m_vm, "soundGet3DPosition", Lua::soundGet3DPosition);
    lua_register(m_vm, "soundSet3DDistance", Lua::soundSet3DDistance);
    lua_register(m_vm, "soundGet3DDistance", Lua::soundGet3DDistance);
    lua_register(m_vm, "soundSetListenerOrientation", Lua::soundSetListenerOrientation);
    lua_register(m_vm, "soundGetListenerOrientation", Lua::soundSetListenerOrientation);
    lua_register(m_vm, "soundSetGlobalVolume", Lua::soundSetGlobalVolume);
    lua_register(m_vm, "soundGetGlobalVolume", Lua::soundGetGlobalVolume);

    //Render Target
    lua_register(m_vm, "rtCreate", Lua::rtCreate);
    lua_register(m_vm, "rtGetSize", Lua::rtGetSize);
    lua_register(m_vm, "rtGetFiltering", Lua::rtGetFiltering);

    //Texture
    lua_register(m_vm, "textureCreate", Lua::textureCreate);
    lua_register(m_vm, "textureGetSize", Lua::textureGetSize);
    lua_register(m_vm, "textureGetFiltering", Lua::textureGetFiltering);

    // Movie
    lua_register(m_vm, "movieCreate", Lua::movieCreate);
    lua_register(m_vm, "moviePlay", Lua::moviePlay);
    lua_register(m_vm, "moviePause", Lua::moviePause);
    lua_register(m_vm, "movieStop", Lua::movieStop);
    lua_register(m_vm, "movieGetSampleRate", Lua::movieGetSampleRate);
    lua_register(m_vm, "movieGetChannelCount", Lua::movieGetChannelCount);
    lua_register(m_vm, "movieGetFramerate", Lua::movieGetFramerate);
    lua_register(m_vm, "movieGetDuration", Lua::movieGetDuration);
    lua_register(m_vm, "movieGetVolume", Lua::movieGetVolume);
    lua_register(m_vm, "movieSetVolume", Lua::movieSetVolume);
    lua_register(m_vm, "movieGetTime", Lua::movieGetTime);
    lua_register(m_vm, "movieSetTime", Lua::movieSetTime);

    // Drawable
    lua_register(m_vm, "drawableDraw", Lua::drawableDraw);
    lua_register(m_vm, "drawableDraw3D", Lua::drawableDraw3D);

    //Font
    lua_register(m_vm, "fontCreate", Lua::fontCreate);
    lua_register(m_vm, "fontDraw", Lua::fontDraw);

    //File
    lua_register(m_vm, "fileCreate", Lua::fileCreate);
    lua_register(m_vm, "fileOpen", Lua::fileOpen);
    lua_register(m_vm, "fileClose", Lua::elementDestroy);
    lua_register(m_vm, "fileRead", Lua::fileRead);
    lua_register(m_vm, "fileWrite", Lua::fileWrite);
    lua_register(m_vm, "fileGetSize", Lua::fileGetSize);
    lua_register(m_vm, "fileSetPosition", Lua::fileSetPosition);
    lua_register(m_vm, "fileGetPosition", Lua::fileGetPosition);
    lua_register(m_vm, "fileFlush", Lua::fileFlush);
    lua_register(m_vm, "fileGetPath", Lua::fileGetPath);
    lua_register(m_vm, "fileIsEOF", Lua::fileIsEOF);
    lua_register(m_vm, "fileDelete", Lua::fileDelete);
    lua_register(m_vm, "fileRename", Lua::fileRename);

    //Collision
    lua_register(m_vm, "collisionCreate", Lua::collisionCreate);
    lua_register(m_vm, "collisionSetPosition", Lua::collisionSetPosition);
    lua_register(m_vm, "collisionGetPosition", Lua::collisionGetPosition);
    lua_register(m_vm, "collisionSetRotation", Lua::collisionSetRotation);
    lua_register(m_vm, "collisionGetRotation", Lua::collisionGetRotation);
    lua_register(m_vm, "collisionSetScale", Lua::collisionSetScale);
    lua_register(m_vm, "collisionGetScale", Lua::collisionGetScale);
    lua_register(m_vm, "collisionGetMass", Lua::collisionGetMass);
    lua_register(m_vm, "collisionSetVelocity", Lua::collisionSetVelocity);
    lua_register(m_vm, "collisionGetVelocity", Lua::collisionGetVelocity);
    lua_register(m_vm, "collisionSetAngularVelocity", Lua::collisionSetAngularVelocity);
    lua_register(m_vm, "collisionGetAngularVelocity", Lua::collisionGetAngularVelocity);
    lua_register(m_vm, "collisionSetFriction", Lua::collisionSetFriction);
    lua_register(m_vm, "collisionGetFriction", Lua::collisionGetFriction);
    lua_register(m_vm, "collisionSetLinearFactor", Lua::collisionSetLinearFactor);
    lua_register(m_vm, "collisionGetLinearFactor", Lua::collisionGetLinearFactor);
    lua_register(m_vm, "collisionSetAngularFactor", Lua::collisionSetAngularFactor);
    lua_register(m_vm, "collisionGetAngularFactor", Lua::collisionGetAngularFactor);
    lua_register(m_vm, "collisionApplyForce", Lua::collisionApplyForce);
    lua_register(m_vm, "collisionApplyImpulse", Lua::collisionApplyImpulse);
    lua_register(m_vm, "collisionApplyTorque", Lua::collisionApplyTorque);
    lua_register(m_vm, "collisionAttach", Lua::collisionAttach);
    lua_register(m_vm, "collisionDetach", Lua::collisionDetach);

    //Elements
    lua_register(m_vm, "isElement", Lua::isElement);
    lua_register(m_vm, "elementGetType", Lua::elementGetType);
    lua_register(m_vm, "elementSetData", Lua::elementSetData);
    lua_register(m_vm, "elementGetData", Lua::elementGetData);
    lua_register(m_vm, "elementRemoveData", Lua::elementRemoveData);
    lua_register(m_vm, "elementDestroy", Lua::elementDestroy);

    //Rendering
    lua_register(m_vm, "setActiveScene", Lua::setActiveScene);
    lua_register(m_vm, "setActiveShader", Lua::setActiveShader);
    lua_register(m_vm, "setRenderTarget", Lua::setRenderTarget);
    lua_register(m_vm, "clearRenderArea", Lua::clearRenderArea);
    lua_register(m_vm, "setClearColor", Lua::setClearColor);
    lua_register(m_vm, "setRenderArea", Lua::setRenderArea);
    lua_register(m_vm, "setPolygonMode", Lua::setPolygonMode);

    //Events
    lua_register(m_vm, "addEvent", Lua::addEvent);
    lua_register(m_vm, "addEventHandler", Lua::addEventHandler);
    lua_register(m_vm, "removeEvent", Lua::removeEvent);
    lua_register(m_vm, "removeEventHandler", Lua::removeEventHandler);
    lua_register(m_vm, "callEvent", Lua::callEvent);

    // User I/O
    lua_register(m_vm, "setCursorMode", Lua::setCursorMode);
    lua_register(m_vm, "setCursorPosition", Lua::setCursorPosition);
    lua_register(m_vm, "getCursorPosition", Lua::getCursorPosition);
    lua_register(m_vm, "getWindowPosition", Lua::getWindowPosition);
    lua_register(m_vm, "getWindowSize", Lua::getWindowSize);
    lua_register(m_vm, "setWindowVSync", Lua::setWindowVSync);
    lua_register(m_vm, "setWindowFramelimit", Lua::setWindowFramelimit);
    lua_register(m_vm, "getWindowFramelimit", Lua::getWindowFramelimit);
    lua_register(m_vm, "setWindowTitle", Lua::setWindowTitle);
    lua_register(m_vm, "setWindowIcon", Lua::setWindowIcon);
    lua_register(m_vm, "requestWindowFocus", Lua::requestWindowFocus);
    lua_register(m_vm, "getWindowFocus", Lua::getWindowFocus);
    lua_register(m_vm, "closeWindow", Lua::closeWindow);
    lua_register(m_vm, "isKeyPressed", Lua::isKeyPressed);
    lua_register(m_vm, "isMouseKeyPressed", Lua::isMouseKeyPressed);
    lua_register(m_vm, "isJoypadConnected", Lua::isJoypadConnected);
    lua_register(m_vm, "joypadGetButtonCount", Lua::joypadGetButtonCount);
    lua_register(m_vm, "joypadGetButtonState", Lua::joypadGetButtonState);
    lua_register(m_vm, "joypadHasAxis", Lua::joypadHasAxis);
    lua_register(m_vm, "joypadGetAxisValue", Lua::joypadGetAxisValue);

    //Bullet Physics
    lua_register(m_vm, "physicsSetEnabled", Lua::physicsSetEnabled);
    lua_register(m_vm, "physicsGetEnabled", Lua::physicsGetEnabled);
    lua_register(m_vm, "physicsSetFloorEnabled", Lua::physicsSetFloorEnabled);
    lua_register(m_vm, "physicsGetFloorEnabled", Lua::physicsGetFloorEnabled);
    lua_register(m_vm, "physicsSetGravity", Lua::physicsSetGravity);
    lua_register(m_vm, "physicsGetGravity", Lua::physicsGetGravity);
    lua_register(m_vm, "physicsRayCast", Lua::physicsRayCast);
    lua_register(m_vm, "physicsSetModelsCollidable", Lua::physicsSetModelsCollidable);

    //System
    lua_register(m_vm, "getTickCount", Lua::getTickCount);
    lua_register(m_vm, "getTime", Lua::getTime);

    //Network
    lua_register(m_vm, "networkConnect", Lua::networkConnect);
    lua_register(m_vm, "networkDisconnect", Lua::networkDisconnect);
    lua_register(m_vm, "networkSendData", Lua::networkSendData);
    lua_register(m_vm, "networkGetState", Lua::networkGetState);
    lua_register(m_vm, "networkGetPing", Lua::networkGetPing);

    m_eventManager = new EventManager(this);
}
ROC::LuaManager::~LuaManager()
{
    lua_close(m_vm);
    delete m_eventManager;
}

bool ROC::LuaManager::OpenFile(const std::string &f_path)
{
    int error = (luaL_loadfile(m_vm, f_path.c_str()) || lua_pcall(m_vm, 0, 0, 0));
    if(error)
    {
        std::string l_log(lua_tostring(m_vm, -1));
        m_core->GetLogManager()->Log(l_log);
        lua_pop(m_vm, 1);
    }
    return (error == 0);
}

void ROC::LuaManager::CallFunction(int f_func, LuaArguments *f_args)
{
    lua_rawgeti(m_vm, LUA_REGISTRYINDEX, f_func);
    f_args->ProccessArguments(m_vm);
    if(lua_pcall(m_vm, f_args->GetArgumentsCount(), 0, 0))
    {
        std::string l_log(lua_tostring(m_vm, -1));
        m_core->GetLogManager()->Log(l_log);
    }
}
