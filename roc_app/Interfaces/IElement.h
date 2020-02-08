#pragma once

namespace ROC
{

class ICustomArgument;

class IElement
{
public:
    enum ElementType : unsigned char
    {
        ET_Geometry = 0U,
        ET_Model,
        ET_Animation,
        ET_Scene,
        ET_Camera,
        ET_Light,
        ET_RenderTarget,
        ET_Shader,
        ET_Sound,
        ET_Texture,
        ET_Font,
        ET_File,
        ET_Collision,

        ET_Invalid = 0xFFU
    };

    virtual unsigned char GetElementType() const = 0;
    virtual const std::string& GetElementTypeName() const = 0;
};

}
