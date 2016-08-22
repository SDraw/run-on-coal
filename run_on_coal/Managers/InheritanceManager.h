#pragma once

namespace ROC
{

class Core;
class Animation;
class Camera;
class Geometry;
class Light;
class Model;
class Scene;
class Shader;
class RenderTarget;
class Texture;
class InheritanceManager
{
    Core *m_core;

    std::multimap<void*,void*> m_inheritMap; //child - parent
    void InheritanceBreakProcessing(void *f_child,void *f_parent,unsigned char f_childType,unsigned char f_parentType);
public:
    bool AttachModelToModel(Model *f_model,Model *f_parent,int f_bone = -1);
    bool DettachModel(Model *f_model);
    bool SetModelAnimation(Model *f_model,Animation *f_anim);
    bool SetSceneCamera(Scene *f_scene,Camera *f_camera);
    bool SetSceneLight(Scene *f_scene,Light *f_light);
    bool AttachTextureToShader(Shader *f_shader,Texture *f_texture,int f_uniform);
    inline void DettachTextureFromShader(Shader *f_shader,Texture *f_texture) { RemoveInheritance(f_texture,f_shader); }
    bool AttachRenderTargetToShader(Shader *f_shader,RenderTarget *f_target,int f_uniform);
    inline void DettachRenderTargetFromShader(Shader *f_shader,RenderTarget *f_target) { RemoveInheritance(f_target,f_shader); }
protected:
    InheritanceManager(Core *f_core);
    ~InheritanceManager();
    void RemoveInheritance(void *f_child,void *f_parent);
    void RemoveChildRelation(void *f_child);
    void RemoveParentRelation(void *f_parent);

    inline void SetModelGeometry(Model *f_model,Geometry *f_geometry) { m_inheritMap.insert(std::pair<void*,void*>(f_model,f_geometry)); }
    friend Core;
    friend class ElementManager;
};

}
