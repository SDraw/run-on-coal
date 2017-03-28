#pragma once

namespace ROC
{

class Core;
class Element;
class Animation;
class Camera;
class Drawable;
class Geometry;
class Light;
class Model;
class Scene;
class Shader;
class InheritanceManager
{
    Core *m_core;

    std::multimap<Element*, Element*> m_inheritMap; //child - parent
    void InheritanceBreakProcessing(Element *f_child, Element *f_parent);
public:
    bool AttachModelToModel(Model *f_model, Model *f_parent, int f_bone = -1);
    bool DettachModel(Model *f_model);
    bool SetModelAnimation(Model *f_model, Animation *f_anim);
    bool SetSceneCamera(Scene *f_scene, Camera *f_camera);
    bool SetSceneLight(Scene *f_scene, Light *f_light);
    bool AttachDrawableToShader(Shader *f_shader, Drawable *f_drawable, const std::string &f_uniform);
    void DettachDrawableFromShader(Shader *f_shader, Drawable *f_drawable);
protected:
    explicit InheritanceManager(Core *f_core);
    ~InheritanceManager();

    void RemoveInheritance(Element *f_child, Element *f_parent);
    void RemoveChildRelation(Element *f_child);
    void RemoveParentRelation(Element *f_parent);

    void SetModelGeometry(Model *f_model, Geometry *f_geometry);

    friend Core;
    friend class ElementManager;
};

}
