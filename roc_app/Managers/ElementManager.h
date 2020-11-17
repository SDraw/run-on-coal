#pragma once
#include "Interfaces/IElementManager.h"

#ifdef CreateFont
#undef CreateFont
#endif

namespace ROC
{

class Core;
class Element;
class Animation;
class Camera;
class Collision;
class Font;
class Geometry;
class Light;
class Model;
class Scene;
class RenderTarget;
class Shader;
class Sound;
class Texture;

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

    //ROC::IElementManager
    IScene* CreateIScene();
    ICamera* CreateICamera(unsigned char f_type);
    ILight* CreateILight(unsigned char f_type);
    IGeometry* CreateIGeometry(const std::string &f_path);
    IModel* CreateIModel(IGeometry *f_geometry);
    IShader* CreateIShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath);
    IAnimation* CreateIAnimation(const std::string &f_path);
    ISound* CreateISound(const std::string &f_path);
    IRenderTarget* CreateIRenderTarget(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter);
    ITexture* CreateITexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress);
    ITexture* CreateITexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);
    IFont* CreateIFont(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter);
    ICollision* CreateICollision(unsigned char f_type, const glm::vec3 &f_size, float f_mass);
    bool IsValidIElement(IElement *f_ptr) const;
    bool DestroyIElement(IElement *f_element);
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
    Font* CreateFont(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter);
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
