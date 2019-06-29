#pragma once

namespace ROC
{

class IVRManager
{
public:
    enum ControllerHandAssigment : unsigned char
    {
        CHA_None = 0U,
        CHA_Left,
        CHA_Right
    };
    enum VREye : unsigned char
    {
        VRE_Left = 0U,
        VRE_Right
    };

    virtual bool IsVREnabled() const = 0;

    virtual const glm::uvec2& GetTargetsSize() const = 0;

    virtual const glm::vec3& GetHeadPosition() const = 0;
    virtual const glm::quat& GetHeadRotation() const = 0;

    virtual const glm::vec3& GetLeftEyePosition() const = 0;
    virtual const glm::vec3& GetRightEyePosition() const = 0;

    virtual bool IsControllerConnected(unsigned int f_id) const = 0;
    virtual bool IsControllerActive(unsigned int f_id) const = 0;
    virtual unsigned char GetControllerHandAssignment(unsigned int f_id) const = 0;
    virtual bool GetControllerPosition(unsigned int f_id, glm::vec3 &f_pos) const = 0;
    virtual bool GetControllerRotation(unsigned int f_id, glm::quat &f_rot) const = 0;
    virtual bool GetControllerVelocity(unsigned int f_id, glm::vec3 &f_val) const = 0;
    virtual bool GetControllerAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const = 0;

    virtual bool ShowNotification(const std::string &f_title, const std::string &f_text, unsigned int f_time) = 0;

    virtual bool DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color) = 0;
    virtual bool DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params) = 0;
};

}
