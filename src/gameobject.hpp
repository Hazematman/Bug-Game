#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "physmesh.hpp"
#include "model.hpp"

enum GameObjectType
{
    GOBJ_PLAYER,
    GOBJ_CRYSTAL,
    GOBJ_TOTEM,
};

class GameObject
{
public:
    void draw(UgfxCommandBuffer &command_queue, float dt=0);

    virtual bool collide(btManifoldPoint &cp, 
                         const btCollisionObjectWrapper *obj1, int id1, int index1) {return false;}

    GameObjectType type;
    bool can_collide;
    Model *model;
    PhysMesh *phys;
    int collision;
};

GameObject *create_crystal(btDiscreteDynamicsWorld *dyn_world);

#endif
