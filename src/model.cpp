#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "model.hpp"

void Model::initalize()
{
    this->num_commands = this->commands_size / sizeof(ugfx_command_t);
    data_cache_hit_writeback(this->verts, this->verts_size);
    data_cache_hit_writeback(this->commands, this->commands_size);
}

void Model::draw(UgfxCommandBuffer &command_queue)
{
    glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), this->position);
    ugfx_matrix_from_column_major(&this->matrix, &model_mat[0][0]);
    data_cache_hit_writeback(&this->matrix, sizeof(this->matrix));

    command_queue.push_back(ugfx_set_model_matrix(0, &this->matrix));
    command_queue.push_back(ugfx_set_address_slot(1, this->verts));
    command_queue.push_back(ugfx_push_commands(0, this->commands, this->num_commands));
    command_queue.push_back(ugfx_sync_pipe());
}
