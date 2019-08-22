#pragma once
#include "Interfaces/IElementManager.h"
#include "Elements/Element.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Camera.h"
#include "Elements/Collision.h"
#include "Elements/Font.h"
#include "Elements/File.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"
#include "Elements/Scene.h"
#include "Elements/Sound.h"
#include "Elements/Texture.h"

namespace ROC
{

class Core;

class ElementManager final : public IElementManager
{
    Core *m_core;

    std::unordered_set<Element*> m_elements;
    std::unordered_set<IElement*> m_interfaces;
    std::unordered_set<Element*>::iterator m_elementsEnd;
    std::unordered_set<IElement*>::iterator m_interfacesEnd;

    void RemoveElementFromSet(Element *f_ptr);

    ElementManager(const ElementManager &that) = delete;
    ElementManager& operator=(const ElementManager &that) = delete;

    // Interfaces reroute
    IModel* CreateModel(IGeometry *f_geometry);
    bool IsValidElement(IElement *f_ptr) const;
    bool DestroyElement(IElement *f_element);
public:
    Scene* CreateScene();
    Camera* CreateCamera(unsigned char f_type);
    Light* CreateLight(unsigned char f_type);
    Geometry* CreateGeometry(const std::string &f_path);
    Model* CreateModel(Geometry *f_geometry);
    Shader* CreateShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath);
    Animation* CreateAnimation(const std::string &f_path);
    Sound* CreateSound(const std::string &f_path);
    RenderTarget* CreateRenderTarget(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter);
    Texture* CreateTexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress);
    Texture* CreateTexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);
    Font* CreateFont_(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter);
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);
    Collision* CreateCollision(unsigned char f_type, const glm::vec3 &f_size, float f_mass);

    bool IsValidElement(Element *f_ptr) const;
    bool DestroyElement(Element *f_element);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    void AddElementToSet(Element *f_ptr);

    friend class Core;
    friend class AsyncManager;
};

}
