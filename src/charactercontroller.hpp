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
    bool bulletCallback(btManifoldPoint &cp, const btCollisionObjectWrapper *obj1, int id1, int index1,
                        const btCollisionObjectWrapper *obj2, int id2, int index2);

    float speed;
    bool got_crystal;
private:
    btDiscreteDynamicsWorld *dyn_world;
    btCollisionShape *ghost_shape;
    btPairCachingGhostObject *ghost;

    /* player state */
    bool injump;

    /* Camera position data */
    glm::vec3 camera_pos;
};

#endif
