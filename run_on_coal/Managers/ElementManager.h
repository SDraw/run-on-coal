#pragma once

namespace ROC
{

class Core;
class Geometry;
class Model;
class Element;
class Animation;
class Scene;
class Camera;
class Light;
class RenderTarget;
class Shader;
class Sound;
class Texture;
class Font;
class File;
class Collision;
class ElementManager
{
    Core *m_core;
    bool m_locked;
public:
    Scene* CreateScene();
    Camera* CreateCamera(int f_type);
    Light* CreateLight();
    Geometry* CreateGeometry(const std::string &f_path);
    Model* CreateModel(Geometry *f_geometry);
    Shader* CreateShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath);
    Animation* CreateAnimation(const std::string &f_path);
    Sound* CreateSound(const std::string &f_path, bool f_loop);
    RenderTarget* CreateRenderTarget(unsigned int f_num, glm::ivec2 &f_size, int f_type, int f_filter);
    Texture* CreateTexture(const std::string &f_path, int f_type, int f_filter, bool f_compress);
    Texture* CreateTexture(const std::vector<std::string> &f_path, int f_filter, bool f_compress);
    Font* CreateFont_(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, int f_filter);
    File* CreateFile_(const std::string &f_path);
    File* OpenFile(const std::string &f_path, bool f_ro);
    Collision* CreateCollision(int f_type, glm::vec3 &f_size);

    void DestroyElement(Element *f_element);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    inline void SetLock(bool f_lock) { m_locked = f_lock; }
    static void DestroyElementByPointer(void* f_pointer);
    friend Core;
    friend class MemoryManager;
};

}
