#pragma once

namespace ROC
{

class IVRManager
{
public:
    virtual bool IsVREnabled() const = 0;

    virtual const glm::uvec2& GetTargetsSize() const = 0;

    virtual const glm::vec3& GetHeadPosition() const = 0;
    virtual const glm::quat& GetHeadRotation() const = 0;

    virtual const glm::vec3& GetLeftEyePosition() const = 0;
    virtual const glm::vec3& GetRightEyePosition() const = 0;

    virtual const bool IsLeftControllerActive() const = 0;
    virtual const glm::vec3& GetLeftControllerPosition() const = 0;
    virtual const glm::quat& GetLeftControllerRotation() const = 0;
    virtual const glm::vec3& GetLeftControllerVelocity() const = 0;
    virtual const glm::vec3& GetLeftControllerAngularVelocity() const = 0;

    virtual const bool IsRightControllerActive() const = 0;
    virtual const glm::vec3& GetRightControllerPosition() const = 0;
    virtual const glm::quat& GetRightControllerRotation() const = 0;
    virtual const glm::vec3& GetRightControllerVelocity() const = 0;
    virtual const glm::vec3& GetRightControllerAngularVelocity() const = 0;
};

}
