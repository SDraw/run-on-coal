#pragma once

namespace ROC
{

class IVRManager
{
public:
    enum DeviceType : unsigned char
    {
        DT_Hmd = 0U,
        DT_Controller,
        DT_Tracker,
        DT_BaseStation,
        DT_Display
    };
    enum HandRole : unsigned char
    {
        HR_Left = 0U,
        HR_Right,
        HR_OptOut,
        HR_Treadmill,
        HR_Stylus
    };
    enum VREye : unsigned char
    {
        VRE_Left = 0U,
        VRE_Right
    };

    virtual bool IsVREnabled() const = 0;
    virtual bool IsDeviceActive(unsigned int f_id) const = 0;
    virtual bool IsDeviceConnected(unsigned int f_id) const = 0;
    virtual bool GetDeviceType(unsigned int f_id, unsigned char &f_type) const = 0;
    virtual bool GetDevicePosition(unsigned int f_id, glm::vec3 &f_pos) const = 0;
    virtual bool GetDeviceRotation(unsigned int f_id, glm::quat &f_rot) const = 0;
    virtual bool GetDeviceVelocity(unsigned int f_id, glm::vec3 &f_val) const = 0;
    virtual bool GetDeviceAngularVelocity(unsigned int f_id, glm::vec3 &f_val) const = 0;
    virtual bool GetDeviceHandRole(unsigned int f_id, unsigned char &f_role) const = 0;
    virtual bool GetDeviceByHandRole(unsigned char f_role, unsigned int &f_id) const = 0;
    virtual bool GetDeviceAxisValues(unsigned int f_id, unsigned int f_axis, glm::vec2 &f_val) const = 0;
    virtual const glm::uvec2& GetTargetsSize() const = 0;
    virtual bool DrawEyeImage(unsigned char f_side, const glm::vec2 &f_pos, const glm::vec2 &f_size, float f_rot, const glm::vec4 &f_color) = 0;
    virtual bool DrawEyeImage(unsigned char f_side, const glm::vec3 &f_pos, const glm::quat &f_rot, const glm::vec2 &f_size, const glm::bvec4 &f_params) = 0;
};

}
