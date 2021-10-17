#ifndef CHARACTERCONTROLLER_HPP
#define CHARACTERCONTROLLER_HPP

#include <libdragon.h>
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "gameobject.hpp"

class CharacterController : public GameObject
{
public:
    CharacterController(btDiscreteDynamicsWorld *dyn_world, glm::vec3 start_pos);
    void update(controller_data &data);
    glm::mat4 getViewMatrix();

    float speed;
private:
    btDiscreteDynamicsWorld *dyn_world;

    /* player state */
    bool injump;

    /* Camera position data */
    glm::vec3 camera_pos;
};

#endif
