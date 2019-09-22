#include "stdafx.h"

#include "Managers/ElementManager.h"
#include "Core/Core.h"
#include "Elements/Animation/Animation.h"
#include "Elements/Camera.h"
#include "Elements/Collision.h"
#include "Elements/File.h"
#include "Elements/Font.h"
#include "Elements/Geometry/Geometry.h"
#include "Elements/Light.h"
#include "Elements/Model/Model.h"
#include "Elements/Scene/Scene.h"
#include "Elements/RenderTarget.h"
#include "Elements/Shader/Shader.h"
#include "Elements/Sound.h"
#include "Elements/Texture.h"

#include "Managers/LogManager.h"
#include "Managers/PhysicsManager.h"
#include "Managers/PreRenderManager.h"
#include "Managers/RenderManager/RenderManager.h"

ROC::ElementManager::ElementManager(Core *f_core)
{
    m_core = f_core;
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}
ROC::ElementManager::~ElementManager()
{
    for(auto l_element : m_elements) delete reinterpret_cast<Element*>(l_element);
    m_elements.clear();
    m_interfaces.clear();
}

void ROC::ElementManager::AddElementToSet(Element *f_ptr)
{
    m_elements.insert(f_ptr);
    m_interfaces.insert(f_ptr);
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}
void ROC::ElementManager::RemoveElementFromSet(Element *f_ptr)
{
    m_elements.erase(f_ptr);
    m_interfaces.erase(f_ptr);
    m_elementsEnd = m_elements.end();
    m_interfacesEnd = m_interfaces.end();
}

ROC::Scene* ROC::ElementManager::CreateScene()
{
    ROC::Scene *l_scene = new Scene();
    AddElementToSet(l_scene);
    return l_scene;
}

ROC::Camera* ROC::ElementManager::CreateCamera(unsigned char f_type)
{
    Camera *l_camera = new Camera(f_type);
    AddElementToSet(l_camera);
    return l_camera;
}

ROC::Light* ROC::ElementManager::CreateLight(unsigned char f_type)
{
    Light *l_light = new Light(f_type);
    AddElementToSet(l_light);
    return l_light;
}

ROC::Animation* ROC::ElementManager::CreateAnimation(const std::string &f_path)
{
    Animation *l_anim = new Animation();

    if(l_anim->Load(f_path)) AddElementToSet(l_anim);
    else
    {
        delete l_anim;
        l_anim = nullptr;
    }
    return l_anim;
}

ROC::Geometry* ROC::ElementManager::CreateGeometry(const std::string &f_path)
{
    Geometry *l_geometry = new Geometry();

    if(l_geometry->Load(f_path))
    {
        l_geometry->GenerateVAOs();
        AddElementToSet(l_geometry);
    }
    else
    {
        delete l_geometry;
        l_geometry = nullptr;
    }
    return l_geometry;
}

ROC::Model* ROC::ElementManager::CreateModel(Geometry *f_geometry)
{
    Model *l_model = new Model(f_geometry);
    AddElementToSet(l_model);
    m_core->GetPreRenderManager()->AddModel(l_model);
    m_core->GetPhysicsManager()->AddModel(l_model);
    return l_model;
}

ROC::Shader* ROC::ElementManager::CreateShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath)
{
    Shader *l_shader = new Shader();

    if(l_shader->Load(f_vpath, f_fpath, f_gpath)) AddElementToSet(l_shader);
    else
    {
        const std::string &l_shaderError = l_shader->GetError();
        if(!l_shaderError.empty())
        {
            std::string l_error("[");
            l_error.append(f_vpath);
            l_error.push_back(',');
            l_error.append(f_fpath);
            l_error.push_back(',');
            l_error.append(f_gpath);
            l_error.append("] -> ");
            l_error.append(l_shaderError);
            m_core->GetLogManager()->Log(l_error);
        }
        delete l_shader;
        l_shader = nullptr;
    }
    return l_shader;
}

ROC::Sound* ROC::ElementManager::CreateSound(const std::string &f_path)
{
    Sound *l_sound = new Sound();

    if(l_sound->Load(f_path)) AddElementToSet(l_sound);
    else
    {
        delete l_sound;
        l_sound = nullptr;
    }
    return l_sound;
}

ROC::RenderTarget* ROC::ElementManager::CreateRenderTarget(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter)
{
    RenderTarget *l_rt = new RenderTarget();

    if(l_rt->Create(f_type, f_size, f_filter)) AddElementToSet(l_rt);
    else
    {
        m_core->GetLogManager()->Log(l_rt->GetError());
        delete l_rt;
        l_rt = nullptr;
    }
    return l_rt;
}

ROC::Texture* ROC::ElementManager::CreateTexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress)
{
    Texture *l_texture = new Texture();

    if(l_texture->Load(f_path, f_type, f_filter, f_compress)) AddElementToSet(l_texture);
    else
    {
        delete l_texture;
        l_texture = nullptr;
    }
    return l_texture;
}

ROC::Texture* ROC::ElementManager::CreateTexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress)
{
    Texture *l_texture = new Texture();

    if(l_texture->LoadCubemap(f_path, f_filter, f_compress)) AddElementToSet(l_texture);
    else
    {
        delete l_texture;
        l_texture = nullptr;
    }
    return l_texture;
}

ROC::Font* ROC::ElementManager::CreateFont_(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter)
{
    Font *l_font = new Font();

    if(l_font->Load(f_path, f_size, f_atlas, f_filter)) AddElementToSet(l_font);
    else
    {
        delete l_font;
        l_font = nullptr;
    }
    return l_font;
}

ROC::File* ROC::ElementManager::CreateFile_(const std::string &f_path)
{
    File *l_file = new File();

    if(l_file->Create(f_path)) AddElementToSet(l_file);
    else
    {
        delete l_file;
        l_file = nullptr;
    }
    return l_file;
}

ROC::File* ROC::ElementManager::OpenFile(const std::string &f_path, bool f_ro)
{
    File *l_file = new File();

    if(l_file->Open(f_path, f_ro)) AddElementToSet(l_file);
    else
    {
        delete l_file;
        l_file = nullptr;
    }
    return l_file;
}

ROC::Collision* ROC::ElementManager::CreateCollision(unsigned char f_type, const glm::vec3 &f_size, float f_mass)
{
    Collision *l_col = new Collision();

    if(l_col->Create(f_type, f_size, f_mass))
    {
        AddElementToSet(l_col);
        m_core->GetPhysicsManager()->AddCollision(l_col);
    }
    else
    {
        delete l_col;
        l_col = nullptr;
    }
    return l_col;
}

bool ROC::ElementManager::IsValidElement(Element *f_ptr) const
{
    auto l_checkIterator = m_elements.find(f_ptr);
    return (l_checkIterator != m_elementsEnd);
}
bool ROC::ElementManager::DestroyElement(Element *f_element)
{
    bool l_result = false;
    if(IsValidElement(f_element))
    {
        switch(f_element->GetElementType())
        {
            case Element::ET_Scene:
                m_core->GetRenderManager()->RemoveAsActiveScene(reinterpret_cast<Scene*>(f_element));
                break;
            case Element::ET_Model:
            {
                m_core->GetPreRenderManager()->RemoveModel(reinterpret_cast<Model*>(f_element));
                m_core->GetPhysicsManager()->RemoveModel(reinterpret_cast<Model*>(f_element));
            } break;
        }

        RemoveElementFromSet(f_element);
        delete f_element;
        l_result = true;
    }
    return l_result;
}

// ROC::IElementManager
bool ROC::ElementManager::IsValidIElement(IElement *f_ptr) const
{
    auto l_checkIterator = m_interfaces.find(f_ptr);
    return (l_checkIterator != m_interfacesEnd);
}
bool ROC::ElementManager::DestroyIElement(IElement *f_element)
{
    bool l_result = false;
    auto l_checkIterator = m_interfaces.find(f_element);
    if(l_checkIterator != m_interfacesEnd) l_result = DestroyElement(dynamic_cast<Element*>(f_element));
    return l_result;
}

ROC::IScene* ROC::ElementManager::CreateIScene()
{
    return CreateScene();
}
ROC::ICamera* ROC::ElementManager::CreateICamera(unsigned char f_type)
{
    return CreateCamera(f_type);
}
ROC::ILight* ROC::ElementManager::CreateILight(unsigned char f_type)
{
    return CreateLight(f_type);
}
ROC::IGeometry* ROC::ElementManager::CreateIGeometry(const std::string &f_path)
{
    return CreateGeometry(f_path);
}
ROC::IModel* ROC::ElementManager::CreateIModel(IGeometry *f_geometry)
{
    return CreateModel(dynamic_cast<Geometry*>(f_geometry));
}
ROC::IShader* ROC::ElementManager::CreateIShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath)
{
    return CreateShader(f_vpath, f_fpath, f_gpath);
}
ROC::IAnimation* ROC::ElementManager::CreateIAnimation(const std::string &f_path)
{
    return CreateAnimation(f_path);
}
ROC::ISound* ROC::ElementManager::CreateISound(const std::string &f_path)
{
    return CreateSound(f_path);
}
ROC::IRenderTarget* ROC::ElementManager::CreateIRenderTarget(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter)
{
    return CreateRenderTarget(f_type, f_size, f_filter);
}
ROC::ITexture* ROC::ElementManager::CreateITexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress)
{
    return CreateTexture(f_path, f_type, f_filter, f_compress);
}
ROC::ITexture* ROC::ElementManager::CreateITexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress)
{
    return CreateTexture(f_path, f_filter, f_compress);
}
ROC::IFont* ROC::ElementManager::CreateIFont(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter)
{
    return CreateFont_(f_path, f_size, f_atlas, f_filter);
}
ROC::IFile* ROC::ElementManager::CreateIFile(const std::string &f_path)
{
    return CreateFile_(f_path);
}
ROC::IFile* ROC::ElementManager::OpenIFile(const std::string &f_path, bool f_ro)
{
    return OpenFile(f_path, f_ro);
}
ROC::ICollision* ROC::ElementManager::CreateICollision(unsigned char f_type, const glm::vec3 &f_size, float f_mass)
{
    return CreateCollision(f_type, f_size, f_mass);
}
