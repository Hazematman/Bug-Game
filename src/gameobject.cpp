#include "gameobject.hpp"
#include "sphere.hpp"

GameObject *create_crystal(btDiscreteDynamicsWorld *dyn_world)
{
    GameObject *out = new GameObject;

    out->type = GOBJ_CRYSTAL;
    out->can_collide = false;
    out->model = new Model;

    out->model->verts = sphere_verts;
    out->model->commands = sphere_commands;
    out->model->verts_size = sphere_verts_length*sizeof(ugfx_vertex_t);
    out->model->commands_size = sphere_commands_length*sizeof(ugfx_command_t);
    out->model->initalize();

    out->phys = new PhysMesh(100.0f, 1.0f, dyn_world, btVector3(-20,10,8), 0xF, 0xF);
    //out->phys->body->setCollisionFlags(out->phys->body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    out->phys->body->setUserPointer(out);

    return out;
}

void GameObject::draw(UgfxCommandBuffer &command_queue, float dt)
{
    if(phys != NULL)
    {
        btTransform trans = phys->body->getWorldTransform();
        btVector3 pos = trans.getOrigin();
        btQuaternion rot = trans.getRotation();
        model->position = glm::vec3(pos[0], pos[1], pos[2]);
        model->rotation = glm::quat(rot[3], rot[0], rot[1], rot[2]);   
    } 

    if(model != NULL)
    {
        model->draw(command_queue, dt);
    }
}
