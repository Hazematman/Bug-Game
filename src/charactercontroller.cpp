#include <libdragon.h>
#include <glm/gtc/matrix_transform.hpp>
#include "charactercontroller.hpp"
#include "beetleBoi.cpp"

CharacterController::CharacterController(btDiscreteDynamicsWorld *dyn_world, glm::vec3 start_pos) :
    dyn_world(dyn_world)
{
    this->type = GOBJ_PLAYER;
    this->can_collide = true;
    float mass = 100.0f;
    this->speed = 10.0f;

    model = new Model;
    model->initalize(&beetleBoi_def);
    model->scale = glm::vec3(0.3f, 0.3f, 0.3f);

    float x_size = (beetleBoi_def.max_x - beetleBoi_def.min_x)*model->scale.x;
    float y_size = (beetleBoi_def.max_y - beetleBoi_def.min_y)*model->scale.y;
    float z_size = (beetleBoi_def.max_z - beetleBoi_def.min_z)*model->scale.z;

    phys = new PhysMesh(mass, btVector3(x_size/2,y_size/2,z_size/2), dyn_world, btVector3(start_pos.x, start_pos.y, start_pos.z),
                        0xf, 0xf);
    phys->body->setAngularFactor(0.0f);
    phys->body->setActivationState(DISABLE_DEACTIVATION);
    phys->body->setUserPointer(this);

    
    ghost_shape = new btBoxShape(btVector3((x_size/2) + 1,(y_size/2), (z_size/2) + 1));
    ghost = new btPairCachingGhostObject();
    ghost->setCollisionShape(ghost_shape);
    ghost->setUserPointer(this);
    ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    ghost->setWorldTransform(phys->body->getWorldTransform());

    dyn_world->addCollisionObject(ghost, btBroadphaseProxy::KinematicFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);

    /* Set player state */
    injump = false;
    carried = NULL;
    interact_button = false;

    /* Start in summer because its the best season */
    season = SEASON_SUMMER;

    /* Set camera position */
    camera_pos = start_pos - glm::vec3(0.0f, -5.0f, -3.0f);
}

void CharacterController::update(controller_data &data)
{
    btTransform player_transform = phys->body->getWorldTransform(); 
    btVector3 player_position = player_transform.getOrigin();

    ghost->setWorldTransform(player_transform);

    float right_value = (float)data.c[0].x / 128.0f;
    float fwd_value = -(float)data.c[0].y / 128.0f;

    glm::vec3 cam_fwd = camera_pos - glm::vec3(player_position[0], player_position[1], player_position[2]);
    cam_fwd.y = 0;
    cam_fwd = glm::normalize(cam_fwd);
    glm::vec3 cam_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam_fwd));

    glm::vec3 analog = fwd_value*cam_fwd + right_value*cam_right;

    btVector3 old_velocity = phys->body->getLinearVelocity();
    
    if(glm::length(analog) > 0.01f)
    {
        glm::vec3 analog_dir = glm::normalize(analog);

        float amount = glm::radians(10.0f);
        btQuaternion new_rot(-right_value*amount, 0, 0);

        player_transform.setRotation((player_transform.getRotation() * new_rot));
        phys->body->setWorldTransform(player_transform);
        phys->body->setLinearVelocity(btVector3(speed*analog_dir.x, old_velocity[1], speed*analog_dir.z));
        phys->body->clearForces();
    }
    else
    {
        // TODO replace this with animation API once its developed
        model->time = 0.0f;

        phys->body->setLinearVelocity(btVector3(0.0f, old_velocity[1], 0.0f));
        phys->body->clearForces();
    }

    /* Raycast to check if player is on ground */
    btVector3 check_position = player_position + btVector3(0, -4, 0);
    btCollisionWorld::ClosestRayResultCallback ray_callback(player_position, check_position);
    /* Only check for collisions with the ground */
    ray_callback.m_collisionFilterMask = 2;
    ray_callback.m_collisionFilterGroup = 2;
    dyn_world->rayTest(player_position, check_position, ray_callback);

    /* Add a jump */
    if(!injump && ray_callback.hasHit() && data.c[0].A)
    {
        btVector3 velocity = phys->body->getLinearVelocity();
        velocity.setY(12.0f);
        phys->body->setLinearVelocity(velocity);
        phys->body->clearForces();
        phys->body->setWorldTransform(player_transform);
        injump = true;
    }

    /* only reset injump value once the player has let go A and touched the ground */ 
    if(ray_callback.hasHit() && injump && !data.c[0].A)
    {
        injump = false;
    }

    btTransform t = player_transform;
    t.setOrigin(btVector3(0,0,0));
    btVector3 fwd = t(btVector3(1.0f, 0.0f, 0.0f));
    glm::vec3 player_pos(player_position[0], player_position[1], player_position[2]);
    glm::vec3 player_fwd(fwd[0], fwd[1], fwd[2]);
    /* Calculate camera position based on new projected position */
    glm::vec3 new_cam_pos = player_pos - (5.0f*player_fwd + glm::vec3(0.0f, -5.0f, 0.0f));

    camera_pos = glm::mix(camera_pos, new_cam_pos, 0.1f);

    if(interact_button && !data.c[0].B)
    {
        interact_button = false;
    }

    if(carried != NULL && carried->type == GOBJ_CRYSTAL)
    {
        if(!interact_button && data.c[0].B)
        {
            carried->phys->body->setLinearVelocity(10.0f*(fwd+btVector3(0.0f, 1.0f, 0.0f)));
            carried->phys->body->setCollisionFlags(carried->phys->body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);

            carried = NULL;

            interact_button = true;
        }
    }

    if(carried != NULL)
    {
        btTransform player_transform = phys->body->getWorldTransform();
        btVector3 origin = player_transform.getOrigin();
        origin[1] += 2.0f;
        player_transform.setOrigin(origin);


        carried->phys->body->setLinearVelocity(btVector3(0,0,0));
        carried->phys->body->clearForces();
        carried->phys->body->setWorldTransform(player_transform);
    }

    ghost->setWorldTransform(phys->body->getWorldTransform());
}

glm::mat4 CharacterController::getViewMatrix()
{
    btTransform player_transform = phys->body->getWorldTransform(); 
    btVector3 player_position = player_transform.getOrigin();
    glm::vec3 player_pos(player_position[0], player_position[1], player_position[2]);

    return glm::lookAt(camera_pos, player_pos, glm::vec3(0.0f, 1.0f, 0.0f));
}

bool CharacterController::collide(btManifoldPoint &cp, 
                                  const btCollisionObjectWrapper *obj1, int id1, int index1)
{
    GameObject *gobj1 = this;
    GameObject *gobj2 = (GameObject*)obj1->getCollisionObject()->getUserPointer();

    /* If either object doesn't have a user pointer exit early as
       we only deal with collision of objects with user pointers */
    if(gobj1 == NULL || gobj2 == NULL)
    {
        return false;
    }

    /* If this is the carried object exit early */
    if(gobj1 == carried || gobj2 == carried)
    {
        return false;
    }

    GameObject *other_obj = (gobj1 == this) ? gobj2 : gobj1;

    if(other_obj->type == GOBJ_CRYSTAL)
    {
        controller_data data;
        controller_read(&data);

        if(!interact_button && carried == NULL && data.c[0].B)
        {
            carried = other_obj; 
            carried->phys->body->setCollisionFlags(carried->phys->body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

            btTransform player_transform = phys->body->getWorldTransform();
            btVector3 origin = player_transform.getOrigin();
            origin[1] += 2.0f;
            player_transform.setOrigin(origin);

            carried->phys->body->setLinearVelocity(btVector3(0,0,0));
            carried->phys->body->clearForces();
            carried->phys->body->setWorldTransform(player_transform);

            interact_button = true;
        }
    }
    else if(other_obj->type == GOBJ_MUSHROOM)
    {
        btTransform &other_trans = other_obj->phys->body->getWorldTransform();
        btVector3 &other_pos = other_trans.getOrigin();
        btTransform &my_trans = phys->body->getWorldTransform();
        btVector3 &my_pos = my_trans.getOrigin();

        float y_size = (model->model_def->max_y - model->model_def->min_y)*model->scale.y;
        float y_origin = model->scale.y*model->model_def->min_y + (y_size/2);
        if(other_pos[1] < (my_pos[1]-y_origin))
        { 
            btVector3 velocity = phys->body->getLinearVelocity();
            velocity.setY(20.0f);
            phys->body->setLinearVelocity(velocity);
            phys->body->clearForces();
            injump = true;
        }
    }

    return false;
}
