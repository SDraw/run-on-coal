#pragma once

namespace ROC
{

class IDrawable;
class IAnimation;
class ICamera;
class ICollision;
class ILight;
class IModel;
class IRenderTarget;
class IScene;
class IShader;

class IInheritanceManager
{
public:
    virtual bool AttachModelToModel(IModel *f_model, IModel *f_parent, int f_bone = -1) = 0;
    virtual bool DetachModel(IModel *f_model) = 0;
    virtual bool SetModelAnimation(IModel *f_model, IAnimation *f_anim) = 0;
    virtual bool RemoveModelAnimation(IModel *f_model) = 0;
    virtual bool SetModelCollision(IModel *f_model, ICollision *f_col) = 0;
    virtual bool RemoveModelCollision(IModel *f_model) = 0;
    virtual bool AddModelToScene(IScene *f_scene, IModel *f_model) = 0;
    virtual bool RemoveModelFromScene(IScene *f_scene, IModel *f_model) = 0;
    virtual bool SetSceneCamera(IScene *f_scene, ICamera *f_camera) = 0;
    virtual bool RemoveSceneCamera(IScene *f_scene) = 0;
    virtual bool AddSceneLight(IScene *f_scene, ILight *f_light) = 0;
    virtual bool RemoveSceneLight(IScene *f_scene, ILight *f_light) = 0;
    virtual bool SetSceneRenderTarget(IScene *f_scene, IRenderTarget*f_rt) = 0;
    virtual bool RemoveSceneRenderTarget(IScene *f_scene) = 0;
    virtual bool SetSceneShader(IScene *f_scene, IShader *f_shader) = 0;
    virtual bool RemoveSceneShader(IScene *f_scene) = 0;
    virtual bool AttachDrawableToShader(IShader *f_shader, IDrawable *f_drawable, const std::string &f_uniform) = 0;
    virtual bool DetachDrawableFromShader(IShader *f_shader, IDrawable *f_drawable) = 0;
};



}