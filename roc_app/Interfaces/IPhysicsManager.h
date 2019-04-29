#pragma once

namespace ROC
{

class IElement;
class ICollision;
class IModel;

class IPhysicsManager
{
public:
    virtual void SetPhysicsEnabled(bool f_value) = 0;
    virtual bool GetPhysicsEnabled() const = 0;
    virtual void SetFloorEnabled(bool f_value) = 0;
    virtual bool GetFloorEnabled() const = 0;
    virtual void SetGravity(const glm::vec3 &f_grav) = 0;
    virtual void GetGravity(glm::vec3 &f_grav) const = 0;

    virtual void SetCollisionScale(ICollision *f_col, const glm::vec3 &f_scale) = 0;
    virtual bool SetModelsCollidable(IModel *f_model1, IModel *f_model2, bool f_state) = 0;
    virtual bool RayCast(const glm::vec3 &f_start, glm::vec3 &f_end, glm::vec3 &f_normal, IElement *&f_element) = 0;
};

}
