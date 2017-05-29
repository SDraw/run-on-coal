#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Movie.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Movie.h"
#include "Utils/Utils.h"

namespace ROC
{
namespace Lua
{

const std::vector<std::string> g_textureTypesTable
{
    "rgb", "rgba", "cube"
};
const std::vector<std::string> g_textureFilteringTypesTable
{
    "nearest", "linear"
};

int movieCreate(lua_State *f_vm)
{
    std::string l_path;
    ArgReader argStream(f_vm);
    argStream.ReadText(l_path);
    if(!argStream.HasErrors() && !l_path.empty())
    {
        Movie *l_movie = LuaManager::GetCore()->GetElementManager()->CreateMovie(l_path);
        l_movie ? argStream.PushPointer(l_movie) : argStream.PushBoolean(false);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int moviePlay(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        l_movie->Play();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int moviePause(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        l_movie->Pause();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}
int movieStop(lua_State *f_vm)
{
    Movie *l_movie;
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_movie);
    if(!argStream.HasErrors())
    {
        l_movie->Stop();
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
