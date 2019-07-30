#include "stdafx.h"

#include "Elements/Collidable.h"

ROC::Collidable::Collidable()
{
}
ROC::Collidable::~Collidable()
{
}

bool ROC::Collidable::SetCollidableWith(Collidable *f_col, bool f_state)
{
    bool l_result = false;
    std::vector<btRigidBody*> l_groupA;
    std::vector<btRigidBody*> l_groupB;

    this->GetRigidBodies(l_groupA);
    f_col->GetRigidBodies(l_groupB);

    if(!l_groupA.empty() && !l_groupB.empty())
    {
        for(auto l_bodyA : l_groupA)
        {
            for(auto l_bodyB : l_groupB)
            {
                l_bodyA->setIgnoreCollisionCheck(l_bodyB, !f_state);
                l_bodyB->setIgnoreCollisionCheck(l_bodyA, !f_state);
            }
        }
        l_result = true;
    }

    return l_result;
}

// Interfaces reroute
bool ROC::Collidable::SetCollidableWith(ICollidable *f_col, bool f_state)
{
    return SetCollidableWith(dynamic_cast<Collidable*>(f_col), f_state);
}
