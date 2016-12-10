#pragma once

namespace ROC
{
enum ElementType : unsigned char
{
    GeometryElement = 0U, ModelElement, AnimationElement, SceneElement,
    CameraElement, LightElement, RenderTargetElement, ShaderElement,
    SoundElement, TextureElement, FontElement, FileElement, CollisionElement
};

class Core;
class Geometry;
class Model;
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
    bool DestroyScene(Scene *f_scene);

    Camera* CreateCamera(unsigned char f_type);
    bool DestroyCamera(Camera *f_cam);

    Light* CreateLight();
    bool DestroyLight(Light *f_light);

    Geometry* CreateGeometry(std::string &f_path);
    bool DestroyGeometry(Geometry *f_geometry);

    Model* CreateModel(Geometry *f_geometry);
    bool DestroyModel(Model *f_model);

    Shader* CreateShader(std::string &f_vpath, std::string &f_fpath, std::string &f_gpath);
    bool DestroyShader(Shader *f_shader);

    Animation* CreateAnimation(std::string &f_path);
    bool DestroyAnimation(Animation *f_anim);

    Sound* CreateSound(std::string &f_path, bool f_loop);
    bool DestroySound(Sound *f_sound);

    RenderTarget* CreateRenderTarget(unsigned int f_num, glm::ivec2 &f_size, int f_type);
    bool DestroyRenderTarget(RenderTarget *f_rt);

    Texture* CreateTexture(std::string &f_path, int f_type, unsigned char f_filter, bool f_compress);
    Texture* CreateTexture(std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress);
    bool DestroyTexture(Texture *f_tex);

    Font* CreateFont_(std::string &f_path, int f_size);
    bool DestroyFont(Font *f_font);

    File* CreateFile_(std::string &f_path);
    File* OpenFile(std::string &f_path, bool f_ro);
    bool DestroyFile(File *f_file);

    Collision* CreateCollision(unsigned char f_type, glm::vec3 &f_size);
    bool DestroyCollision(Collision *f_col);
protected:
    explicit ElementManager(Core *f_core);
    ~ElementManager();

    inline void SetLock(bool f_lock) { m_locked = f_lock; }
    void DestroyByPointer(void* f_pointer, unsigned char f_type);
    friend Core;
    friend class MemoryManager;
};
}
