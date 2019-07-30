#pragma once
#include "Interfaces/ICollidable.h"
#include "Elements/Element.h"

namespace ROC
{

class Collidable : public Element, public virtual ICollidable
{
    Collidable(const Collidable &that) = delete;
    Collidable& operator=(const Collidable &that) = delete;

    // Interfaces reroute
    bool SetCollidableWith(ICollidable *f_col, bool f_state);
public:
    bool SetCollidableWith(Collidable *f_col, bool f_state);
protected:
    Collidable();
    virtual ~Collidable();

    virtual void GetRigidBodies(std::vector<btRigidBody*> &f_vec) = 0;
};

}

