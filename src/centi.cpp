#include <stdlib.h>
#include "centi_body.cpp"
#include "centi.hpp"
#include "head.cpp"
#include "body.cpp"

Centi::Centi(btDiscreteDynamicsWorld *dyn_world, glm::vec3 start_pos, GameObject *target)
{
    length = 1;
    ate = 0;
    speed = 5.0f;

    this->dyn_world = dyn_world;
    this->target = target;

    target->phys->body->setActivationState(DISABLE_DEACTIVATION);

    btVector3 start(start_pos.x, start_pos.y, start_pos.z);
    // Initalize all the body elements
    for(int i=0; i < MAX_BODY_LENGTH; i++)
    {
        body[i].type = GOBJ_CENTI;
        body[i].can_collide = false;
        body[i].model = new Model;

        if(i == 0)
        {
            body[i].model->initalize(&head_def);
        }
        else
        {
            body[i].model->initalize(&body_def);
        }
        
        body[i].phys = new PhysMesh(100.0f, 1.0f, dyn_world, btVector3(100,-100,100), 0x2, 0xd);
        body[i].phys->body->setUserPointer(this);
        body[i].phys->body->setAngularFactor(0.0f);
        if(i != 0)
        {
            dyn_world->removeRigidBody(body[i].phys->body);
        }
    }

    btTransform t = body[0].phys->body->getWorldTransform();
    t.setOrigin(start);
    body[0].phys->body->setWorldTransform(t);
    body[0].phys->body->setActivationState(DISABLE_DEACTIVATION);
}

void Centi::add_body()
{
    // Don't add a body if we're at the max
    if(length >= MAX_BODY_LENGTH)
    {
        return;
    }

    GameObject &pb = body[length-1];
    GameObject &nb = body[length];
    
    btVector3 pos = pb.phys->body->getWorldTransform().getOrigin();
    pos[1] += 1;

    btTransform t = nb.phys->body->getWorldTransform();
    t.setOrigin(pos);
    dyn_world->addRigidBody(nb.phys->body, 0xd, 0x2);

    nb.phys->body->setWorldTransform(t);
    nb.phys->body->setActivationState(DISABLE_DEACTIVATION);
    nb.phys->body->clearForces();

    length++;
}

void Centi::update(float dt)
{
    UNUSED(dt);
    btTransform main_t = body[0].phys->body->getWorldTransform();
    btVector3 main_pos = main_t.getOrigin();

    btTransform target_t = target->phys->body->getWorldTransform();
    btVector3 target_pos = target_t.getOrigin();

    btVector3 main_dir = target_pos - main_pos;

    if(main_dir.norm() < 2.1f)
    {
        ate += 1;
        speed += 0.5f;

        if(speed > 10.0f)
            speed = 10.0f;

        if(length < MAX_BODY_LENGTH && (ate % 4) == 0)
        {
            speed = 5.0f;
            // We can grow!
            add_body();
        }

        float x = (rand() % 56) - 28.0f;
        float y = (rand() % 56) - 28.0f;

        btTransform t = target->phys->body->getWorldTransform();
        t.setOrigin(btVector3(x, 10, y));
        target->phys->body->setWorldTransform(t); 
        target->phys->body->clearForces();
    }

    main_dir[1] = 0.0f;
    main_dir = main_dir.normalize();

    float rot = atan2f(main_dir[0], main_dir[2]); 
    btQuaternion new_rot(rot, 0, 0);

    main_t.setRotation(new_rot);
    body[0].phys->body->setWorldTransform(main_t);

    btVector3 old_velocity = body[0].phys->body->getLinearVelocity();

    // Raycast to check if player is on ground
    btVector3 check_position = main_pos + 1.2f*main_dir;
    btCollisionWorld::ClosestRayResultCallback ray_callback(main_pos, check_position);
    // Only check for collisions with the ground
    ray_callback.m_collisionFilterMask = 2;
    ray_callback.m_collisionFilterGroup = 2;
    dyn_world->rayTest(main_pos, check_position, ray_callback);

    if(ray_callback.hasHit())
    {
        body[0].phys->body->setLinearVelocity(btVector3(speed*main_dir[0], 8.0f, speed*main_dir[2]));
    }
    else
    {
        body[0].phys->body->setLinearVelocity(btVector3(speed*main_dir[0], old_velocity[1], speed*main_dir[2]));
    }

    body[0].phys->body->clearForces();

    for(int i=1; i < length; i++)
    {
        btTransform main_t = body[i].phys->body->getWorldTransform();
        btVector3 main_pos = main_t.getOrigin();

        btTransform target_t = body[i-1].phys->body->getWorldTransform();
        btVector3 target_pos = target_t.getOrigin();

        btVector3 main_dir = target_pos - main_pos;
        main_dir = main_dir.normalize();

        body[i].phys->body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
        body[i].phys->body->clearForces();

        float rot = atan2f(main_dir[0], main_dir[2]); 
        btQuaternion new_rot(rot, 0, 0);

        main_t.setRotation(new_rot);

        main_t.setOrigin(-2*main_dir + target_pos);
        body[i].phys->body->setWorldTransform(main_t);
    }
}

void Centi::draw(UgfxCommandBuffer &command_queue, float dt)
{
    for(int i=0; i < length; i++)
    {
        body[i].draw(command_queue, dt);
    }
}
