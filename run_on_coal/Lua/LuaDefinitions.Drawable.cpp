#include "stdafx.h"
#include "Core/Core.h"
#include "Managers/ElementManager.h"
#include "Managers/LuaManager.h"
#include "Managers/MemoryManager.h"
#include "Managers/RenderManager/RenderManager.h"
#include "Elements/Drawable.h"
#include "Lua/ArgReader.h"
#include "Lua/LuaDefinitions.Drawable.h"

namespace ROC
{
namespace Lua
{

int drawableDraw(lua_State *f_vm)
{
    Drawable *l_drawable;
    glm::vec2 l_pos, l_size;
    float l_rot = 0.f;
    glm::vec4 l_color(1.f);
    ArgReader argStream(f_vm);
    argStream.ReadElement(l_drawable);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_pos[i]);
    for(int i = 0; i < 2; i++) argStream.ReadNumber(l_size[i]);
    argStream.ReadNextNumber(l_rot);
    for(int i = 0; i < 4; i++) argStream.ReadNextNumber(l_color[i]);
    if(!argStream.HasErrors())
    {
        LuaManager::GetCore()->GetRenderManager()->Render(l_drawable, l_pos, l_size, l_rot, l_color);
        argStream.PushBoolean(true);
    }
    else argStream.PushBoolean(false);
    return argStream.GetReturnValue();
}

}
}
