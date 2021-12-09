#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "model.hpp"

void Model::initalize()
{
    model_def = NULL;
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    this->num_commands = this->commands_size / sizeof(ugfx_command_t);
    data_cache_hit_writeback(this->verts, this->verts_size);
    data_cache_hit_writeback(this->commands, this->commands_size);
}

void Model::initalize(ModelDef *model_def)
{
    this->model_def = model_def;
    this->position = glm::vec3(0,0,0);
    this->rotation = glm::quat();
    this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
    this->time = 0;
}

void Model::draw(UgfxCommandBuffer &command_queue, float dt)
{
    if(model_def != NULL)
    {
        if(model_def->num_animations > 0)
        {
            // TODO don't hardcode this, get the length of current animation and use it here
            time = fmodf(time+dt, 20.0f / 24.0f);
            int frame_slot = 0;
            int next_frame_slot = 0;
            float lerp_time = 0;
            for(int i=0; i < (int)model_def->animation_group->num_animations; i++)
            {
                Animation *anim = model_def->animation_group->animations;
                float keyframe_time = anim[i].keyframe / 24.0f;
                if(keyframe_time <= time)
                {
                    frame_slot = i;
                    next_frame_slot = (i + 1) % model_def->animation_group->num_animations;
                    float next_frame_time = anim[next_frame_slot].keyframe / 24.0f;
                    lerp_time = (time - keyframe_time) / (next_frame_time - keyframe_time); 
                }
            }

            for(int i=0; i < (int)model_def->num_meshes; i++)
            {
                AnimationTrans *trans = model_def->animation_group->animations[frame_slot].trans;
                AnimationTrans *trans2 = model_def->animation_group->animations[next_frame_slot].trans;

                glm::vec3 mesh_pos = glm::vec3(model_def->meshes[i].position[0], model_def->meshes[i].position[1], model_def->meshes[i].position[2]);
                glm::vec3 bone_offset = glm::vec3(trans[i].position[0], trans[i].position[1], trans[i].position[2]);
                glm::vec3 bone_offset2 = glm::vec3(trans2[i].position[0], trans2[i].position[1], trans2[i].position[2]);
                bone_offset = glm::mix(bone_offset, bone_offset2, lerp_time);
                glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), position);
                glm::mat4 rot_mat = glm::mat4_cast(glm::normalize(this->rotation));

                glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);

                glm::mat4 bone_offset_mat = glm::translate(glm::mat4(1.0f), bone_offset);

                glm::mat4 offset_mat_inv = glm::translate(glm::mat4(1.0f), -mesh_pos);
                glm::mat4 offset_mat = glm::translate(glm::mat4(1.0f), mesh_pos);
                glm::quat rot(trans[i].rotation[3], trans[i].rotation[0], trans[i].rotation[1], trans[i].rotation[2]);
                glm::quat rot2(trans2[i].rotation[3], trans2[i].rotation[0], trans2[i].rotation[1], trans2[i].rotation[2]);
                rot = glm::mix(glm::normalize(rot), glm::normalize(rot2), lerp_time);

                glm::mat4 out_mat = model_mat * rot_mat * scale * bone_offset_mat * offset_mat * glm::mat4(rot) * offset_mat_inv;
                ugfx_matrix_from_column_major(&model_def->meshes[i].matrix, &out_mat[0][0]);
                data_cache_hit_writeback(&model_def->meshes[i].matrix, sizeof(model_def->meshes[i].matrix));

                command_queue.push_back(ugfx_set_model_matrix(0, &model_def->meshes[i].matrix));
                command_queue.push_back(ugfx_push_commands(0, model_def->meshes[i].commands, model_def->meshes[i].num_commands));
                command_queue.push_back(ugfx_sync_pipe());
            }
        } 
        else
        {
            // If the model has no animation just draw each mesh of the model using the default transformations
            for(int i=0; i < (int)model_def->num_meshes; i++)
            {
                glm::vec3 mesh_pos = glm::vec3(model_def->meshes[i].position[0], model_def->meshes[i].position[1], model_def->meshes[i].position[2]);
                glm::mat4 model_mat = glm::translate(glm::mat4(1.0f), position);
                glm::mat4 rot_mat = glm::mat4_cast(glm::normalize(this->rotation));
                glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);
                glm::mat4 offset_mat = glm::translate(glm::mat4(1.0f), mesh_pos);

                glm::mat4 out_mat = model_mat * rot_mat * scale * offset_mat;
                ugfx_matrix_from_column_major(&matrix, &out_mat[0][0]);
                data_cache_hit_writeback(&matrix, sizeof(matrix));

                command_queue.push_back(ugfx_set_model_matrix(0, &matrix));
                command_queue.push_back(ugfx_push_commands(0, model_def->meshes[i].commands, model_def->meshes[i].num_commands));
                command_queue.push_back(ugfx_sync_pipe());
            }
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
