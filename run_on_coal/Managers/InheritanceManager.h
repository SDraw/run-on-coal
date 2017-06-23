#pragma once

namespace ROC
{

class Core;
class Element;
class Collision;
class Animation;
class Camera;
class Drawable;
class Geometry;
class Light;
class Model;
class Scene;
class Shader;
class InheritanceManager final
{
    Core *m_core;

    std::multimap<Element*, Element*> m_inheritMap; //child - parent

    void AddInheritance(Element *f_child, Element *f_parent);
    void RemoveInheritance(Element *f_child, Element *f_parent);

    void InheritanceBreakProcessing(Element *f_child, Element *f_parent);
public:
    bool AttachModelToModel(Model *f_model, Model *f_parent, int f_bone = -1);
    bool DetachModel(Model *f_model);
    bool SetModelAnimation(Model *f_model, Animation *f_anim);
    bool RemoveModelAnimation(Model *f_model);

    bool AttachCollisionToModel(Collision *f_col, Model *f_model);
    bool DetachCollision(Collision *f_col);

    bool SetSceneCamera(Scene *f_scene, Camera *f_camera);
    bool RemoveSceneCamera(Scene *f_scene);
    bool SetSceneLight(Scene *f_scene, Light *f_light);
    bool RemoveSceneLight(Scene *f_scene);

    bool AttachDrawableToShader(Shader *f_shader, Drawable *f_drawable, const std::string &f_uniform);
    bool DetachDrawableFromShader(Shader *f_shader, Drawable *f_drawable);
protected:
    explicit InheritanceManager(Core *f_core);
    ~InheritanceManager();

    void RemoveChildRelations(Element *f_child);
    void RemoveParentRelations(Element *f_parent);

    void SetModelGeometry(Model *f_model, Geometry *f_geometry);

    friend class Core;
    friend class ElementManager;
};

}
