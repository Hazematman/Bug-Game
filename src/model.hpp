#ifndef MODEL_HPP
#define MODEL_HPP
#include <libdragon.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "internal.hpp"
#include "mesh.hpp"

class Model
{
public:
    ugfx_vertex_t *verts;
    ugfx_command_t *commands;
    size_t verts_size;
    size_t commands_size;
    uint32_t num_commands;
    glm::vec3 position;
    glm::vec3 scale;
    glm::quat rotation;
    ugfx_matrix_t matrix;
    ModelDef *model_def;

    float time;

    void initalize();
    void initalize(ModelDef *model_def);
    void draw(UgfxCommandBuffer &command_queue, float dt=0);
};

#endif
