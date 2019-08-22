#pragma once

class CustomArgument;

namespace ROC
{

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

    virtual bool SetData(const std::string &f_key, const CustomArgument &f_val) = 0;
    virtual bool GetData(const std::string &f_key, CustomArgument &f_val) = 0;
    virtual bool RemoveData(const std::string &f_key) = 0;

    virtual unsigned char GetElementType() const = 0;
    virtual const std::string& GetElementTypeName() const = 0;
};

}
