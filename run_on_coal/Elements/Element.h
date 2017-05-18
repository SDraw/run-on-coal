#pragma once
#include "Utils/CustomData.h"

namespace ROC
{

class Element
{
    std::unordered_map<std::string, CustomData> m_customDataMap;
    std::unordered_map<std::string, CustomData>::iterator m_customDataMapEnd;
public:
    enum ElementType : unsigned char
    {
        GeometryElement = 0U, ModelElement, AnimationElement, SceneElement,
        CameraElement, LightElement, RenderTargetElement, ShaderElement,
        SoundElement, TextureElement, FontElement, FileElement, CollisionElement
    };

    bool SetCustomData(const std::string &f_key, CustomData &f_val);
    void GetCustomData(const std::string &f_key, CustomData &f_val);
    bool RemoveCustomData(const std::string &f_key);

    inline unsigned char GetElementType() const { return m_elementType; }
protected:
    unsigned char m_elementType;

    Element();
    virtual ~Element();

    friend class ElementManager;
};

}
