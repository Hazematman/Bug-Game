#ifndef CENTI_HPP
#define CENTI_HPP

#include "gameobject.hpp"

#define MAX_BODY_LENGTH 8

class Centi
{
public:
    Centi(btDiscreteDynamicsWorld *dyn_world, glm::vec3 start_pos, GameObject *target);

    GameObject *target;
    GameObject body[MAX_BODY_LENGTH];
    int length;
    float speed;
    int ate;
    void update(float dt);
    void draw(UgfxCommandBuffer &command_queue, float dt);
private:
    btDiscreteDynamicsWorld *dyn_world;
    void add_body();
};

#endif
