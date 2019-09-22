#pragma once

namespace ROC
{

class IElement;
class IAnimation;
class ICamera;
class ICollision;
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
    virtual IScene* CreateIScene() = 0;
    virtual ICamera* CreateICamera(unsigned char f_type) = 0;
    virtual ILight* CreateILight(unsigned char f_type) = 0;
    virtual IGeometry* CreateIGeometry(const std::string &f_path) = 0;
    virtual IModel* CreateIModel(IGeometry *f_geometry) = 0;
    virtual IShader* CreateIShader(const std::string &f_vpath, const std::string &f_fpath, const std::string &f_gpath) = 0;
    virtual IAnimation* CreateIAnimation(const std::string &f_path) = 0;
    virtual ISound* CreateISound(const std::string &f_path) = 0;
    virtual IRenderTarget* CreateIRenderTarget(unsigned char f_type, const glm::ivec2 &f_size, unsigned char f_filter) = 0;
    virtual ITexture* CreateITexture(const std::string &f_path, unsigned char f_type, unsigned char f_filter, bool f_compress) = 0;
    virtual ITexture* CreateITexture(const std::vector<std::string> &f_path, unsigned char f_filter, bool f_compress) = 0;
    virtual IFont* CreateIFont(const std::string &f_path, int f_size, const glm::ivec2 &f_atlas, unsigned char f_filter) = 0;
    virtual IFile* CreateIFile(const std::string &f_path) = 0;
    virtual IFile* OpenIFile(const std::string &f_path, bool f_ro) = 0;
    virtual ICollision* CreateICollision(unsigned char f_type, const glm::vec3 &f_size, float f_mass) = 0;

    virtual bool IsValidIElement(IElement *f_ptr) const = 0;
    virtual bool DestroyIElement(IElement *f_element) = 0;
};

}
