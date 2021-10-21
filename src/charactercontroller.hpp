#ifndef CHARACTERCONTROLLER_HPP
#define CHARACTERCONTROLLER_HPP

#include <libdragon.h>
#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include "gameobject.hpp"

enum Season
{
    SEASON_SPRING,
    SEASON_SUMMER,
    SEASON_FALL,
    SEASON_WINTER,
};

class CharacterController : public GameObject
{
public:
    CharacterController(btDiscreteDynamicsWorld *dyn_world, glm::vec3 start_pos);
    void update(controller_data &data);
    virtual bool collide(btManifoldPoint &cp, 
                         const btCollisionObjectWrapper *obj1, int id1, int index1);
    glm::mat4 getViewMatrix();

    float speed;
    bool got_crystal;
    Season season;
private:
    btDiscreteDynamicsWorld *dyn_world;
    btCollisionShape *ghost_shape;
    btPairCachingGhostObject *ghost;

    /* player state */
    bool injump;
    bool interact_button;
    GameObject *carried;

    /* Camera position data */
    glm::vec3 camera_pos;
};

#endif
