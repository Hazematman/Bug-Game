#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "cube.hpp"
#include "model.hpp"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;


static display_context_t disp = 0;
UgfxCommandBuffer disp_commands(UGFX_CMDBUF_SIZE);

uint8_t depth_buffer[DISPLAY_WIDTH*DISPLAY_HEIGHT*2] __attribute__ ((aligned (64)));

class Player
{
    public:
        glm::vec3 pos;
        glm::vec3 fwd;
};

int main(void)
{
    init_interrupts();

    debug_init_usblog();
    debug_init_isviewer();

    /* Initalize basics */
    controller_init();
    display_init(res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    ugfx_init(UGFX_DEFAULT_RDP_BUFFER_SIZE);
    dfs_init(DFS_DEFAULT_LOCATION);

    ugfx_viewport_t viewport;
    ugfx_viewport_init(&viewport, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    data_cache_hit_writeback(&viewport, sizeof(viewport));

    Player player;
    player.pos = glm::vec3(0.0f, 0.0f, -3.0f);
    player.fwd = glm::vec3(0.0f, 0.0f, -1.0f);

    Model cube_model;
    Model cube2;

    cube_model.verts = mesh_vertices;
    cube_model.commands = mesh_commands;
    cube_model.commands_size = mesh_commands_length;
    cube_model.verts_size = mesh_vertices_length; 

    cube_model.position = glm::vec3(0.0f, 0.0f, -3.0f);
    cube_model.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    cube2.verts = mesh_vertices;
    cube2.commands = mesh_commands;
    cube2.commands_size = mesh_commands_length;
    cube2.verts_size = mesh_vertices_length; 

    cube2.position = glm::vec3(0.0f, 0.0f, -3.0f);
    cube2.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    cube_model.initalize();
    cube2.initalize();

    float near_plane = 0.1f;
    float far_plane = 100.0f;

    ugfx_matrix_t pv_matrix;
    auto mat = glm::perspective(glm::radians(90.0f), 320.0f/240.0f, near_plane, far_plane);

    uint16_t perspective_normalization_scale = float_to_fixed(get_persp_norm_scale(near_plane, far_plane), 16);

    glm::vec3 camera_pos = player.pos - glm::vec3(0.0f, 3.0f, -3.0f);
    struct controller_data data;
    while(1)
    {
        controller_read(&data);
        glm::vec3 cam_fwd = camera_pos - player.pos;
        cam_fwd.y = 0;
        cam_fwd = glm::normalize(cam_fwd);
        glm::vec3 cam_right = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam_fwd));

        float right_value = (float)data.c[0].x / 1024.0f;
        float fwd_value = -(float)data.c[0].y / 1024.0f;
        glm::vec3 analog = fwd_value*cam_fwd + right_value*cam_right; 
        glm::vec3 analog_dir = glm::normalize(analog);

        if(glm::length(analog) > 0.01f)
        {
            float yaw = glm::degrees(atan(-analog_dir.z / analog_dir.x));

            player.fwd = analog_dir;
            player.pos += analog;
            cube_model.rotation.y = yaw;
        }

        glm::vec3 new_cam_pos = player.pos - (5.0f*player.fwd + glm::vec3(0.0f, 2.0f, 0.0f));

        camera_pos = glm::mix(camera_pos, new_cam_pos, 0.1f);
        cube_model.position = player.pos;

        glm::mat4 view_mat = glm::lookAt(camera_pos, player.pos, glm::vec3(0.0f, 1.0f, 0.0f));;
        glm::mat4 pv = mat * view_mat;
        ugfx_matrix_from_column_major(&pv_matrix, &pv[0][0]);
        data_cache_hit_writeback(&pv_matrix, sizeof(pv_matrix));


        while( !(disp = display_lock()) );

        disp_commands.clear();

        disp_commands.push_back(ugfx_set_scissor(0,0, DISPLAY_WIDTH << 2, DISPLAY_HEIGHT << 2, UGFX_SCISSOR_DEFAULT));
        disp_commands.push_back(ugfx_load_viewport(0, &viewport));
        disp_commands.push_back(ugfx_set_z_image(depth_buffer));
        disp_commands.push_back(ugfx_set_other_modes(UGFX_CYCLE_FILL));
        disp_commands.push_back(ugfx_set_color_image(depth_buffer, UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, DISPLAY_WIDTH-1));
        disp_commands.push_back(ugfx_set_fill_color(PACK_ZDZx2(0xFFFF, 0)));
        disp_commands.push_back(ugfx_fill_rectangle(0, 0, DISPLAY_WIDTH << 2, DISPLAY_HEIGHT << 2));

        disp_commands.push_back(ugfx_set_display(disp));
        disp_commands.push_back(ugfx_set_fill_color(PACK_RGBA16x2(40, 20, 255, 255)));
        disp_commands.push_back(ugfx_fill_rectangle(0, 0, DISPLAY_WIDTH << 2, DISPLAY_HEIGHT << 2));

        disp_commands.push_back(ugfx_set_view_persp_matrix(0, &pv_matrix));
        disp_commands.push_back(ugfx_set_persp_norm(perspective_normalization_scale));

        disp_commands.push_back(ugfx_set_other_modes(
                UGFX_CYCLE_1CYCLE |
                ugfx_blend_1cycle(UGFX_BLEND_IN_RGB, UGFX_BLEND_IN_ALPHA, UGFX_BLEND_MEM_RGB, UGFX_BLEND_1_MINUS_A) |
                UGFX_SAMPLE_2x2 | UGFX_Z_OPAQUE | UGFX_Z_SOURCE_PIXEL | UGFX_CVG_CLAMP | UGFX_BI_LERP_0 | UGFX_BI_LERP_1 |
                UGFX_Z_COMPARE | UGFX_Z_UPDATE | UGFX_PERSP_TEX | UGFX_ALPHA_CVG_SELECT | UGFX_IMAGE_READ | UGFX_ANTIALIAS
                ));
        disp_commands.push_back(ugfx_set_combine_mode(
                UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_PRIM_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1,
                UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_PRIM_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1
                ));
        disp_commands.push_back(ugfx_set_cull_mode(UGFX_CULL_FRONT));
        disp_commands.push_back(ugfx_set_geometry_mode( UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_SMOOTH));
        disp_commands.push_back(ugfx_set_prim_color(0, 0, PACK_RGBA32(255, 0, 0, 255)));
        disp_commands.push_back(ugfx_set_clip_ratio(2));

        cube_model.draw(disp_commands);

        disp_commands.push_back(ugfx_set_prim_color(0, 0, PACK_RGBA32(0, 255, 0, 255)));

        cube2.draw(disp_commands);

        disp_commands.push_back(ugfx_sync_full());
        disp_commands.push_back(ugfx_finalize());

        data_cache_hit_writeback(&disp_commands[0], disp_commands.size()*sizeof(ugfx_command_t));
        uint32_t num_commands = disp_commands.size();

        debugf("number of commands %d\r\n", (int)num_commands);

        ugfx_load(&disp_commands[0], num_commands);
        rsp_run();

        display_show(disp);
    }
}
