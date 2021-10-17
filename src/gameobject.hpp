#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP
#include "physmesh.hpp"
#include "model.hpp"

enum GameObjectType
{
    GOBJ_PLAYER,
    GOBJ_CRYSTAL,
};

class GameObject
{
public:
    void draw(UgfxCommandBuffer &command_queue);

    GameObjectType type;
    Model *model;
    PhysMesh *phys;
};

GameObject *create_crystal(btDiscreteDynamicsWorld *dyn_world);

#endif
