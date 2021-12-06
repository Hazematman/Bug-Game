#include "mushroom.hpp"
#include "shroom.cpp"

struct frame_data
{
    float time;
    glm::vec3 scale;
};

frame_data frames[] =
{
    {0.0f, glm::vec3(1.0f, 1.0f, 1.0f)},
    {0.1f, glm::vec3(1.5f, 0.5f, 1.5f)},
    {0.2f, glm::vec3(0.5f, 1.5f, 0.5f)},
    {0.3f, glm::vec3(1.0f, 1.0f, 1.0f)},
};

Mushroom::Mushroom(btVector3 origin, btDiscreteDynamicsWorld *dyn_world)
{
    type = GOBJ_MUSHROOM;
    model = new Model;

    model->initalize(&shroom_def);

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

    jump = false;
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
    static int num_frames = sizeof(frames) / sizeof(*frames);
    if(jump)
    {
        float next_time = fmodf(anim_time+dt, frames[num_frames-1].time);
        int frame_slot = 0;
        int next_frame_slot = 0;
        float lerp_time = 0;
        for(int i=0; i < num_frames; i++)
        {
            float keyframe_time = frames[i].time;
            if(keyframe_time <= next_time)
            {
                frame_slot = i;
                next_frame_slot = (i + 1) % num_frames;
                float next_frame_time = frames[next_frame_slot].time;
                lerp_time = (next_time - keyframe_time) / (next_frame_time - keyframe_time); 
            }
        }

        model->scale = glm::mix(frames[frame_slot].scale, frames[next_frame_slot].scale, lerp_time);

        if(anim_time > next_time)
        {
            jump = false;
            model->scale = glm::vec3(1.0f, 1.0f, 1.0f);
        }
        else
        {
            anim_time = next_time;
        }
    }
}
