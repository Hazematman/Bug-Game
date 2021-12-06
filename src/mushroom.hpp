#ifndef MUSHROOM_HPP
#define MUSHROOM_HPP
#include "gameobject.hpp"

class Mushroom : public GameObject
{
public:
    Mushroom(btVector3 origin, btDiscreteDynamicsWorld *dyn_world);
    virtual bool collide(btManifoldPoint &cp, 
                         const btCollisionObjectWrapper *obj1, int id1, int index1);
    virtual void update(float dt);

    bool jump;
    float anim_time;
private:
    btDiscreteDynamicsWorld *dyn_world;
    btCollisionShape *ghost_shape;
    btPairCachingGhostObject *ghost;
};

#endif
