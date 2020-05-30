#pragma once

namespace ROC
{

class IVRManager
{
public:
    enum ControllerHandAssigment : unsigned char
    {
        CHA_Left = 0U,
        CHA_Right,
        CHA_Any
    };
    enum VREye : unsigned char
    {
        VRE_Left = 0U,
        VRE_Right
    };

    virtual bool IsVREnabled() const = 0;
    virtual const glm::uvec2& GetTargetsSize() const = 0;
    virtual const glm::vec3& GetHmdPosition() const = 0;
    virtual const glm::quat& GetHmdRotation() const = 0;
    virtual bool IsControllerConnected(unsigned int f_id) const = 0;
    virtual bool IsControllerActive(unsigned int f_id) const = 0;
    virtual unsigned char GetControllerHand(unsigned int f_id) const = 0;
    virtual unsigned int GetControllerFromHand(unsigned char f_hand) = 0;
    virtual bool GetControllerPosition(unsigned int f_id, glm::vec3 &f_pos) const = 0;
    virtual bool GetControllerRotation(unsigned int f_id, glm::quat &f_rot) const = 0;
    virtual bool GetControllerVelocity(unsigned int f_id, glm::vec3 &f_val) const = 0;
    virtual bool GetControllerAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const = 0;
    virtual bool DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color, const std::string &f_layer) = 0;
    virtual bool DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const std::string &f_layer, const glm::bvec4 &f_params) = 0;
};

}
