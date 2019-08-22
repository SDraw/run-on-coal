#pragma once

namespace ROC
{

class IAnimation;
class ICamera;
class ICollision;
class IElement;
class IFile;
class IFont;
class IGeometry;
class ILight;
class IModel;
class IScene;
class IRenderTarget;
class IShader;
class ISound;
class ITexture;

class IElementManager
{
public:
    virtual IScene* CreateScene() = 0;
    virtual ICamera* CreateCamera(unsigned char f_type) = 0;
    virtual ILight* CreateLight(unsigned char f_type) = 0;
    virtual IGeometry* CreateGeometry(const std::string &f_path) = 0;
    virtual IModel* CreateModel(IGeometry *f_geometry) = 0;
    virtual IShader* CreateShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath) = 0;
    virtual IAnimation* CreateAnimation(const std::string &f_path) = 0;
    virtual ISound* CreateSound(const std::string &f_path) = 0;
    virtual IRenderTarget* CreateRenderTarget(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter) = 0;
    virtual ITexture* CreateTexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress) = 0;
    virtual ITexture* CreateTexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress) = 0;
    virtual IFont* CreateFont_(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter) = 0;
    virtual IFile* CreateFile_(const std::string &f_path) = 0;
    virtual IFile* OpenFile(const std::string &f_path, bool f_ro) = 0;
    virtual ICollision* CreateCollision(unsigned char f_type, const glm::vec3 &f_size, float f_mass) = 0;

    virtual bool IsValidElement(IElement *f_ptr) const = 0;
    virtual bool DestroyElement(IElement *f_element) = 0;
};

}
