#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "model.hpp"

void Model::initalize()
{
    model_def = NULL;
    this->num_commands = this->commands_size / sizeof(ugfx_command_t);
    data_cache_hit_writeback(this->verts, this->verts_size);
    data_cache_hit_writeback(this->commands, this->commands_size);
}

void Model::initalize(ModelDef *model_def)
{
    this->model_def = model_def;
    this->position = glm::vec3(0,0,0);
    this->rotation = glm::quat();
}

void Model::draw(UgfxCommandBuffer &command_queue, int frame)
{
    if(model_def != NULL)
    {
        // TODO program framerate into model def;
        frame = frame % 24;
        int frame_slot = 0;
        for(int i=0; i < (int)model_def->animation_group->num_animations; i++)
        {
            Animation *anim = model_def->animation_group->animations;
            if(anim[i].keyframe <= frame)
            {
                frame_slot = i;
            }
        }
        for(int i=0; i < (int)model_def->num_meshes; i++)
        {
            AnimationTrans *trans = model_def->animation_group->animations[frame_slot].trans;
            glm::vec3 mesh_pos = glm::vec3(model_def->meshes[i].position[0], model_def->meshes[i].position[1], model_def->meshes[i].position[2]);
            glm::vec3 bone_offset = glm::vec3(trans[i].position[0], trans[i].position[1], trans[i].position[2]);
            UNUSED(mesh_pos);
            UNUSED(bone_offset);
            glm::vec3 new_pos = position + bone_offset;
            glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), new_pos);
            glm::mat4 offset_mat_inv = glm::translate(glm::mat4(1.0f), -mesh_pos);
            glm::mat4 offset_mat = glm::translate(glm::mat4(1.0f), mesh_pos);
            glm::mat4 rot = glm::eulerAngleYXZ(trans[i].rotation[1], trans[i].rotation[0], trans[i].rotation[2]);
            UNUSED(rot);
            // TODO probably need matrix per mesh
            //model_mat = model_mat * glm::mat4_cast(glm::normalize(this->rotation));
            //model_mat = model_mat * rot;
            model_mat = model_mat * offset_mat * rot * offset_mat_inv;
            ugfx_matrix_from_column_major(&model_def->meshes[i].matrix, &model_mat[0][0]);
            data_cache_hit_writeback(&model_def->meshes[i].matrix, sizeof(model_def->meshes[i].matrix));

            command_queue.push_back(ugfx_set_model_matrix(0, &model_def->meshes[i].matrix));
            command_queue.push_back(ugfx_push_commands(0, model_def->meshes[i].commands, model_def->meshes[i].num_commands));
            command_queue.push_back(ugfx_sync_pipe());
        }
    }
    else
    {
        glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), this->position);
        model_mat = model_mat * glm::mat4_cast(glm::normalize(this->rotation));
        ugfx_matrix_from_column_major(&this->matrix, &model_mat[0][0]);
        data_cache_hit_writeback(&this->matrix, sizeof(this->matrix));

        command_queue.push_back(ugfx_set_model_matrix(0, &this->matrix));
        command_queue.push_back(ugfx_set_address_slot(1, this->verts));
        command_queue.push_back(ugfx_push_commands(0, this->commands, this->num_commands));
        command_queue.push_back(ugfx_sync_pipe());
    }
}
