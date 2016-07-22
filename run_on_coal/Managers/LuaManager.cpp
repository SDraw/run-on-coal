#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/EventManager.h"
#include "Managers/LogManager.h"
#include "Managers/LuaManager.h"
#include "Lua/LuaArguments.h"
#include "Lua/LuaDefinitions.Animation.h"
#include "Lua/LuaDefinitions.Camera.h"
#include "Lua/LuaDefinitions.Collision.h"
#include "Lua/LuaDefinitions.Events.h"
#include "Lua/LuaDefinitions.Font.h"
#include "Lua/LuaDefinitions.Glfw.h"
#include "Lua/LuaDefinitions.Light.h"
#include "Lua/LuaDefinitions.Geometry.h"
#include "Lua/LuaDefinitions.Model.h"
#include "Lua/LuaDefinitions.OpenGL.h"
#include "Lua/LuaDefinitions.Physics.h"
#include "Lua/LuaDefinitions.Rendering.h"
#include "Lua/LuaDefinitions.RenderTarget.h"
#include "Lua/LuaDefinitions.Scene.h"
#include "Lua/LuaDefinitions.Shader.h"
#include "Lua/LuaDefinitions.Sound.h"
#include "Lua/LuaDefinitions.Texture.h"
#include "Lua/LuaDefinitions.Cursor.h"
#include "Lua/LuaDefinitions.File.h"
#include "Lua/LuaDefinitions.Utils.h"

ROC::Core* ROC::LuaManager::m_corePointer = NULL;

ROC::LuaManager::LuaManager(Core *f_core)
{
    m_core = f_core;
    m_corePointer = f_core;

    m_pVM = luaL_newstate();
    luaL_requiref(m_pVM,"_G",luaopen_base,1);
    luaL_requiref(m_pVM,"math",luaopen_math,1);
    luaL_requiref(m_pVM,"string",luaopen_string,1);
    luaL_requiref(m_pVM,"table",luaopen_table,1);
    luaL_requiref(m_pVM,"bit32",luaopen_bit32,1);
    luaL_requiref(m_pVM,"utf8",luaopen_utf8,1);

    //Security?
    lua_register(m_pVM,"dofile",Lua::disabledFunction);
    lua_register(m_pVM,"loadfile",Lua::disabledFunction);

    //Log
    lua_register(m_pVM,"logPrint",Lua::logPrint);

    //Geometry
    lua_register(m_pVM,"geometryCreate",Lua::geometryCreate);
    lua_register(m_pVM,"geometryDestroy",Lua::geometryDestroy);

    //Model
    lua_register(m_pVM,"modelCreate",Lua::modelCreate);
    lua_register(m_pVM,"modelDestroy",Lua::modelDestroy);
    lua_register(m_pVM,"modelGetGeometry",Lua::modelGetGeometry);
    lua_register(m_pVM,"modelGetType",Lua::modelGetType);
    lua_register(m_pVM,"modelSetPosition",Lua::modelSetPosition);
    lua_register(m_pVM,"modelGetPosition",Lua::modelGetPosition);
    lua_register(m_pVM,"modelSetRotation",Lua::modelSetRotation);
    lua_register(m_pVM,"modelGetRotation",Lua::modelGetRotation);
    lua_register(m_pVM,"modelSetScale",Lua::modelSetScale);
    lua_register(m_pVM,"modelGetScale",Lua::modelGetScale);
    lua_register(m_pVM,"modelSetMatrixUpdate",Lua::modelSetMatrixUpdate);
    lua_register(m_pVM,"modelDraw",Lua::modelDraw);
    lua_register(m_pVM,"modelAttach",Lua::modelAttach);
    lua_register(m_pVM,"modelDettach",Lua::modelDettach);
    lua_register(m_pVM,"modelGetParent",Lua::modelGetParent);
    lua_register(m_pVM,"modelSetAnimation",Lua::modelSetAnimation);
    lua_register(m_pVM,"modelGetAnimation",Lua::modelGetAnimation);
    lua_register(m_pVM,"modelPlayAnimation",Lua::modelPlayAnimation);
    lua_register(m_pVM,"modelPauseAnimation",Lua::modelPauseAnimation);
    lua_register(m_pVM,"modelResetAnimation",Lua::modelResetAnimation);
    lua_register(m_pVM,"modelSetAnimationSpeed",Lua::modelSetAnimationSpeed);
    lua_register(m_pVM,"modelGetAnimationSpeed",Lua::modelGetAnimationSpeed);
    lua_register(m_pVM,"modelSetAnimationProgress",Lua::modelSetAnimationProgress);
    lua_register(m_pVM,"modelGetAnimationProgress",Lua::modelGetAnimationProgress);
    lua_register(m_pVM,"modelSetBonePosition",Lua::modelSetBonePosition);
    lua_register(m_pVM,"modelSetBoneRotation",Lua::modelSetBoneRotation);
    lua_register(m_pVM,"modelSetBoneScale",Lua::modelSetBoneScale);
    lua_register(m_pVM,"modelSetRigidity",Lua::modelSetRigidity);
    lua_register(m_pVM,"modelRemoveRigidity",Lua::modelRemoveRigidity);
    lua_register(m_pVM,"modelGetMass",Lua::modelGetMass);
    lua_register(m_pVM,"modelSetFriction",Lua::modelSetFriction);
    lua_register(m_pVM,"modelGetFriction",Lua::modelGetFriction);
    lua_register(m_pVM,"modelSetVelocity",Lua::modelSetVelocity);
    lua_register(m_pVM,"modelGetVelocity",Lua::modelGetVelocity);
    lua_register(m_pVM,"modelSetAngularVelocity",Lua::modelSetAngularVelocity);
    lua_register(m_pVM,"modelGetAngularVelocity",Lua::modelGetAngularVelocity);

    //Shader
    lua_register(m_pVM,"shaderCreate",Lua::shaderCreate);
    lua_register(m_pVM,"shaderDestroy",Lua::shaderDestroy);
    lua_register(m_pVM,"shaderGetUniform",Lua::shaderGetUniform);
    lua_register(m_pVM,"shaderSetUniformValue",Lua::shaderSetUniformValue);

    //Scene
    lua_register(m_pVM,"sceneCreate",Lua::sceneCreate);
    lua_register(m_pVM,"sceneDestroy",Lua::sceneDestroy);
    lua_register(m_pVM,"sceneSetCamera",Lua::sceneSetCamera);
    lua_register(m_pVM,"sceneGetCamera",Lua::sceneGetCamera);
    lua_register(m_pVM,"sceneSetLight",Lua::sceneSetLight);
    lua_register(m_pVM,"sceneGetLight",Lua::sceneGetLight);

    //Camera
    lua_register(m_pVM,"cameraCreate",Lua::cameraCreate);
    lua_register(m_pVM,"cameraDestroy",Lua::cameraDestroy);
    lua_register(m_pVM,"cameraSetPosition",Lua::cameraSetPosition);
    lua_register(m_pVM,"cameraGetPosition",Lua::cameraGetPosition);
    lua_register(m_pVM,"cameraSetDirection",Lua::cameraSetDirection);
    lua_register(m_pVM,"cameraGetDirection",Lua::cameraGetDirection);
    lua_register(m_pVM,"cameraSetPerspective",Lua::cameraSetPerspective);
    lua_register(m_pVM,"cameraSetOrtho",Lua::cameraSetOrtho);
    lua_register(m_pVM,"cameraGetViewMatrix",Lua::cameraGetViewMatrix);
    lua_register(m_pVM,"cameraGetProjectionMatrix",Lua::cameraGetProjectionMatrix);

    //Light
    lua_register(m_pVM,"lightCreate",Lua::lightCreate);
    lua_register(m_pVM,"lightDestroy",Lua::lightDestroy);
    lua_register(m_pVM,"lightSetParams",Lua::lightSetParams);
    lua_register(m_pVM,"lightGetParams",Lua::lightGetParams);
    lua_register(m_pVM,"lightSetColor",Lua::lightSetColor);
    lua_register(m_pVM,"lightGetColor",Lua::lightGetColor);
    lua_register(m_pVM,"lightSetDirection",Lua::lightSetDirection);
    lua_register(m_pVM,"lightGetDirection",Lua::lightGetDirection);

    //Animation
    lua_register(m_pVM,"animationCreate",Lua::animationCreate);
    lua_register(m_pVM,"animationDestroy",Lua::animationDestroy);

    //Sound
    lua_register(m_pVM,"soundCreate",Lua::soundCreate);
    lua_register(m_pVM,"soundDestroy",Lua::soundDestroy);
    lua_register(m_pVM,"soundPlay",Lua::soundPlay);
    lua_register(m_pVM,"soundPause",Lua::soundPause);
    lua_register(m_pVM,"soundStop",Lua::soundStop);
    lua_register(m_pVM,"soundIsLooped",Lua::soundIsLooped);
    lua_register(m_pVM,"soundGetState",Lua::soundGetState);
    lua_register(m_pVM,"soundSetSpeed",Lua::soundSetSpeed);
    lua_register(m_pVM,"soundGetSpeed",Lua::soundGetSpeed);
    lua_register(m_pVM,"soundSetVolume",Lua::soundSetVolume);
    lua_register(m_pVM,"soundGetVolume",Lua::soundGetVolume);
    lua_register(m_pVM,"soundSetTime",Lua::soundSetTime);
    lua_register(m_pVM,"soundGetTime",Lua::soundGetTime);
    lua_register(m_pVM,"soundSet3DEnabled",Lua::soundSet3DEnabled);
    lua_register(m_pVM,"soundGet3DEnabled",Lua::soundGet3DEnabled);
    lua_register(m_pVM,"soundSet3DPosition",Lua::soundSet3DPosition);
    lua_register(m_pVM,"soundGet3DPosition",Lua::soundGet3DPosition);
    lua_register(m_pVM,"soundSet3DDistance",Lua::soundSet3DDistance);
    lua_register(m_pVM,"soundGet3DDistance",Lua::soundGet3DDistance);
    lua_register(m_pVM,"soundSetListenerOrientation",Lua::soundSetListenerOrientation);
    lua_register(m_pVM,"soundGetListenerOrientation",Lua::soundSetListenerOrientation);

    //Render Target
    lua_register(m_pVM,"rtCreate",Lua::rtCreate);
    lua_register(m_pVM,"rtDestroy",Lua::rtDestroy);
    lua_register(m_pVM,"rtDraw",Lua::rtDraw);

    //Texture
    lua_register(m_pVM,"textureCreate",Lua::textureCreate);
    lua_register(m_pVM,"textureDestroy",Lua::textureDestroy);
    lua_register(m_pVM,"textureDraw",Lua::textureDraw);

    //Font
    lua_register(m_pVM,"fontCreate",Lua::fontCreate);
    lua_register(m_pVM,"fontDestroy",Lua::fontDestroy);
    lua_register(m_pVM,"fontDraw",Lua::fontDraw);

    //Cursor
    lua_register(m_pVM,"cursorCreate",Lua::cursorCreate);
    lua_register(m_pVM,"cursorDestroy",Lua::cursorDestroy);

    //File
    lua_register(m_pVM,"fileCreate",Lua::fileCreate);
    lua_register(m_pVM,"fileOpen",Lua::fileOpen);
    lua_register(m_pVM,"fileClose",Lua::fileClose);
    lua_register(m_pVM,"fileRead",Lua::fileRead);
    lua_register(m_pVM,"fileWrite",Lua::fileWrite);
    lua_register(m_pVM,"fileGetSize",Lua::fileGetSize);
    lua_register(m_pVM,"fileSetPosition",Lua::fileSetPosition);
    lua_register(m_pVM,"fileGetPosition",Lua::fileGetPosition);
    lua_register(m_pVM,"fileGetPath",Lua::fileGetPath);
    lua_register(m_pVM,"fileIsEOF",Lua::fileIsEOF);
    lua_register(m_pVM,"fileDelete",Lua::fileDelete);
    lua_register(m_pVM,"fileRename",Lua::fileRename);

    //Collision
    lua_register(m_pVM,"collisionCreate",Lua::collisionCreate);
    lua_register(m_pVM,"collisionDestroy",Lua::collisionDestroy);
    lua_register(m_pVM,"collisionSetPosition",Lua::collisionSetPosition);
    lua_register(m_pVM,"collisionGetPosition",Lua::collisionGetPosition);
    lua_register(m_pVM,"collisionSetRotation",Lua::collisionSetRotation);
    lua_register(m_pVM,"collisionGetRotation",Lua::collisionGetRotation);

    //Rendering
    lua_register(m_pVM,"setActiveScene",Lua::setActiveScene);
    lua_register(m_pVM,"setActiveShader",Lua::setActiveShader);
    lua_register(m_pVM,"setRenderTarget",Lua::setRenderTarget);

    //Events
    lua_register(m_pVM,"addEvent",Lua::addEvent);
    lua_register(m_pVM,"removeEvent",Lua::removeEvent);

    //OpenGL
    lua_register(m_pVM,"oglClear",Lua::oglClear);
    lua_register(m_pVM,"oglClearColor",Lua::oglClearColor);
    lua_register(m_pVM,"oglViewport",Lua::oglViewport);
    lua_register(m_pVM,"oglPolygonMode",Lua::oglPolygonMode);

    //GLFW
    lua_register(m_pVM,"setCursor",Lua::setCursor);
    lua_register(m_pVM,"setCursorMode",Lua::setCursorMode);
    lua_register(m_pVM,"setCursorPosition",Lua::setCursorPosition);
    lua_register(m_pVM,"getCursorPosition",Lua::getCursorPosition);
    lua_register(m_pVM,"getTime",Lua::getTime);
    lua_register(m_pVM,"getWindowSize",Lua::getWindowSize);
    lua_register(m_pVM,"closeApplication",Lua::closeApp);

    //Bullet Physics
    lua_register(m_pVM,"physicsSetEnabled",Lua::physicsSetEnabled);
    lua_register(m_pVM,"physicsGetEnabled",Lua::physicsGetEnabled);
    lua_register(m_pVM,"physicsSetFloorEnabled",Lua::physicsSetFloorEnabled);
    lua_register(m_pVM,"physicsGetFloorEnabled",Lua::physicsGetFloorEnabled);
    lua_register(m_pVM,"physicsSetGravity",Lua::physicsSetGravity);
    lua_register(m_pVM,"physicsGetGravity",Lua::physicsGetGravity);
    lua_register(m_pVM,"physicsRayCast",Lua::physicsRayCast);

    //Elements
    lua_register(m_pVM,"isElement",Lua::isElement);
    lua_register(m_pVM,"getElementType",Lua::getElementType);

    m_eventManager = new EventManager(this);
}

ROC::LuaManager::~LuaManager()
{
    lua_close(m_pVM);
    delete m_eventManager;
}

bool ROC::LuaManager::OpenFile(std::string &f_path)
{
    int error = luaL_loadfile(m_pVM,f_path.c_str()) || lua_pcall(m_pVM,0,0,0);
    if(error)
    {
        std::string l_log;
        l_log.append(lua_tostring(m_pVM,-1));
        m_core->GetLogManager()->Log(l_log);
        lua_pop(m_pVM,1);
        return false;
    }
    return true;
}

void ROC::LuaManager::CallFunction(int f_func,LuaArguments &f_args)
{
    lua_rawgeti(m_pVM,LUA_REGISTRYINDEX,f_func);
    f_args.ProccessArguments(m_pVM);
    if(lua_pcall(m_pVM,static_cast<int>(f_args.GetArgumentsValue()),0,0)) 
    {
        std::string l_log;
        l_log.append(lua_tostring(m_pVM,-1));
        m_core->GetLogManager()->Log(l_log);
    }
}

ROC::EventManager* ROC::LuaManager::GetEventManager() 
{
    return m_eventManager;
}

void ROC::LuaManager::RemoveReference(int f_ref)
{
    luaL_unref(m_pVM,LUA_REGISTRYINDEX,f_ref);
}
