#ifndef TOTEM_HPP
#define TOTEM_HPP
#include "gameobject.hpp"

class Totem : public GameObject
{
public:
    Totem(btVector3 origin, btDiscreteDynamicsWorld *dyn_world);
    virtual bool collide(btManifoldPoint &cp, 
                         const btCollisionObjectWrapper *obj1, int id1, int index1);

    GameObject *carried;
private:
    btDiscreteDynamicsWorld *dyn_world;
    btCollisionShape *ghost_shape;
    btPairCachingGhostObject *ghost;
};

#endif
