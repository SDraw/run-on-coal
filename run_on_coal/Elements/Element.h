#pragma once

namespace ROC
{

class Element
{
public:
    enum ElementType : unsigned char
    {
        GeometryElement = 0U, ModelElement, AnimationElement, SceneElement,
        CameraElement, LightElement, RenderTargetElement, ShaderElement,
        SoundElement, TextureElement, FontElement, FileElement, CollisionElement
    };
    inline unsigned char GetElementType() const { return m_elementType; }
protected:
    unsigned char m_elementType;

    Element();
    virtual ~Element();

    friend class ElementManager;
};

}
