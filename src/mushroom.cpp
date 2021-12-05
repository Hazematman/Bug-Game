#include "mushroom.hpp"
#include "cube.hpp"

Mushroom::Mushroom(btVector3 origin, btDiscreteDynamicsWorld *dyn_world)
{
    type = GOBJ_MUSHROOM;
    model = new Model;

    model->verts = mesh_vertices;
    model->commands = mesh_commands;
    model->verts_size = mesh_vertices_length;
    model->commands_size = mesh_commands_length;
    model->position = glm::vec3(origin[0], origin[1], origin[2]);
    model->rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    model->initalize();

    phys = new PhysMesh(0.0f, btVector3(1,1,1), dyn_world, origin, 0xF, 0xF);
    phys->body->setAngularFactor(0.0f);
    phys->body->setUserPointer(this);

    ghost_shape = new btBoxShape(btVector3(1,1,1));
    ghost = new btPairCachingGhostObject();
    ghost->setCollisionShape(ghost_shape);
    ghost->setUserPointer(this);
    ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    ghost->setWorldTransform(phys->body->getWorldTransform());

    dyn_world->addCollisionObject(ghost, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
}

bool Mushroom::collide(btManifoldPoint &cp, 
                       const btCollisionObjectWrapper *obj1, int id1, int index1)
{
    UNUSED(cp);
    UNUSED(obj1);
    UNUSED(id1);
    UNUSED(index1);
    return false;
}


void Mushroom::update(float dt)
{
    UNUSED(dt);
}
