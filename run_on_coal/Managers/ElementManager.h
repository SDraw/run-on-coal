#pragma once

namespace ROC
{

enum ElementType
{
    GeometryElement = 1U, ModelElement, AnimationElement, SceneElement,
    CameraElement, LightElement, RenderTargetElement, ShaderElement,
    SoundElement, TextureElement, FontElement, CursorElement, FileElement, CollisionElement
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
class Cursor;
class File;
class Collision;
class ElementManager
{
    Core *m_core;
    bool m_locked;
public:
    Scene* CreateScene();
    bool DestroyScene(Scene *f_scene);

    Camera* CreateCamera();
    bool DestroyCamera(Camera *f_cam);

    Light* CreateLight();
    bool DestroyLight(Light *f_light);

    Geometry* CreateGeometry(std::string &f_path, bool l_comp);
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

    Texture* CreateTexture(std::string &f_path, int f_type, bool f_compress = false);
    Texture* CreateTexture(std::vector<std::string> &f_path, bool f_compress = false);
    bool DestroyTexture(Texture *f_tex);

    Font* CreateFont_(std::string &f_path, int f_size);
    bool DestroyFont(Font *f_font);

    Cursor* CreateCursor(std::string &f_path, bool f_sys);
    bool DestroyCursor(Cursor *f_cursor);

    File* CreateFile_(std::string &f_path);
    File* OpenFile(std::string &f_path,bool f_ro);
    bool DestroyFile(File *f_file);

    Collision* CreateCollision(unsigned char f_type, glm::vec3 &f_size);
    bool DestroyCollision(Collision *f_col);
protected:
    ElementManager(Core *f_core);
    ~ElementManager();
    void SetLock(bool f_lock);
    void DestroyByPointer(void* f_pointer,unsigned char f_type);
    friend Core;
    friend class MemoryManager;
};

}
