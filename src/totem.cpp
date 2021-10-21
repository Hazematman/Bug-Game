#include "totem.hpp"
#include "totem_mdl.hpp"
#include "charactercontroller.hpp"

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

    ghost_shape = new btBoxShape(btVector3(2,3.5f,2));
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

void Totem::update(float dt)
{
    /* Totem doesn't do anything based on dt */
    UNUSED(dt);

    if(carried)
    {
        btTransform totem_transform = phys->body->getWorldTransform();
        btVector3 origin = totem_transform.getOrigin();
        origin[1] += 7.0f;
        totem_transform.setOrigin(origin);

        carried->phys->body->setLinearVelocity(btVector3(0,0,0));
        carried->phys->body->clearForces();
        carried->phys->body->setWorldTransform(totem_transform);
    }
}

bool Totem::collide(btManifoldPoint &cp, 
                    const btCollisionObjectWrapper *obj1, int id1, int index1)
{
    GameObject *gobj = (GameObject*)obj1->getCollisionObject()->getUserPointer();

    /* If we already carrying an object exit early */
    if(carried)
        return false;

    if(gobj->type == GOBJ_CRYSTAL)
    {
        carried = gobj; 
        carried->phys->body->setCollisionFlags(carried->phys->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

        btTransform totem_transform = phys->body->getWorldTransform();
        btVector3 origin = totem_transform.getOrigin();
        origin[1] += 7.0f;
        totem_transform.setOrigin(origin);

        carried->phys->body->setLinearVelocity(btVector3(0,0,0));
        carried->phys->body->clearForces();
        carried->phys->body->setWorldTransform(totem_transform);

        global_character->season = SEASON_WINTER;
    }
    
    return false;
}
