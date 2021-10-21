#include "totem.hpp"
#include "totem_mdl.hpp"

Totem::Totem(btVector3 origin, btDiscreteDynamicsWorld *dyn_world)
{
    type = GOBJ_TOTEM;
    model = new Model[10];

    model->verts = totem_mdl_verts;
    model->commands = totem_mdl_commands;
    model->verts_size = totem_mdl_verts_length*sizeof(ugfx_vertex_t);
    model->commands_size = totem_mdl_commands_length*sizeof(ugfx_command_t);
    model->initalize();

    phys = new PhysMesh(0.0f, btVector3(1,3,1), dyn_world, origin, 0xF, 0xF);
    phys->body->setAngularFactor(0.0f);
    phys->body->setUserPointer(this);

    ghost_shape = new btBoxShape(btVector3(2,1,2));
    ghost = new btPairCachingGhostObject();
    ghost->setCollisionShape(ghost_shape);
    ghost->setUserPointer(this);
    ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    ghost->setWorldTransform(phys->body->getWorldTransform());

    dyn_world->addCollisionObject(ghost, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

    carried = NULL;
    can_collide = true;
    collision = 0;
}

bool Totem::collide(btManifoldPoint &cp, 
                    const btCollisionObjectWrapper *obj1, int id1, int index1)
{
    collision += 1;
    return false;
}
