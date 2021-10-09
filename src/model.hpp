#ifndef MODEL_HPP
#define MODEL_HPP
#include <libdragon.h>
#include <glm/glm.hpp>
#include "internal.hpp"

class Model
{
public:
    ugfx_vertex_t *verts;
    ugfx_command_t *commands;
    size_t verts_size;
    size_t commands_size;
    uint32_t num_commands;
    glm::vec3 position;
    glm::vec3 rotation;
    ugfx_matrix_t matrix;

    void initalize();
    void draw(UgfxCommandBuffer &command_queue);
};

#endif
