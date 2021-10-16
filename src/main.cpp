#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
extern "C" {
#include <libdragon.h>
};
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include "cube.hpp"
#include "model.hpp"
#include "physmesh.hpp"
#include "charactercontroller.hpp"

#include "test_level_2.hpp"
#include "sphere.hpp"
#include "snow.hpp"


wav64_t Casio_SA_76_Piano1;
#include "midstream.hpp"
#include "out.hpp"

#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 240

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_16_BPP;


static display_context_t disp = 0;
UgfxCommandBuffer disp_commands(UGFX_CMDBUF_SIZE);

uint8_t depth_buffer[DISPLAY_WIDTH*DISPLAY_HEIGHT*2] __attribute__ ((aligned (64)));

void my_ugfx_viewport_init(ugfx_viewport_t *viewport, int16_t left, int16_t top, int16_t right, int16_t bottom)
{
    left = left << 2;
    top = top << 2;
    right = right << 2;
    bottom = bottom << 2;

    int16_t half_width = (right - left) / 2;
    int16_t half_height = (bottom - top) / 2;

    *viewport = (ugfx_viewport_t) {
        .scale = { 
            half_width,
            (int16_t)(-half_height),
            Z_MAX / 2,
            0
        },
        .offset = { 
            (int16_t)(left + half_width),
            (int16_t)(bottom - half_height),
            Z_MAX / 2,
            0 
        }
    };
}

volatile bool dp_sync_status;

void dp_sync()
{
    dp_sync_status = true;
}

int mixer_event(void *ctx)
{
    static int index = 0;

    int next_index = (index + 1) % (sizeof(test_song_stream) / sizeof(*test_song_stream));

    float next_ts = test_song_stream[next_index].delta_time; 
    int num_samples = next_ts * 44100.0f;

    for(int i=0; i < (int)test_song_stream[index].num_events; i++)
    {
        MixEvent &event = test_song_stream[index].events[i];
        if(!event.start)
        {
            mixer_ch_stop(event.channel);
        }
        else
        {
            mixer_ch_play(event.channel, &event.sample->wave);
            mixer_ch_set_vol(event.channel, event.volume, event.volume);
            mixer_ch_set_freq(event.channel, event.freq);
        }
    }

    /* Increase index with looping around song */
    index = next_index;

    return num_samples == 0 ? 1 : num_samples;
}

int main(void)
{
    init_interrupts();

    //debug_init_usblog();
    //debug_init_isviewer();

    /* Initalize basics */
    controller_init();
    display_init(res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    ugfx_init(UGFX_DEFAULT_RDP_BUFFER_SIZE);
    dfs_init(DFS_DEFAULT_LOCATION);

    register_DP_handler(dp_sync);
    set_DP_interrupt(true);

    audio_init(44100, 8);
    mixer_init(16);

    for(int i = 0; i < 16; i++)
        mixer_ch_set_limits(i, 0, 128000, 0);

    wav64_open(&Casio_SA_76_Piano1, "Casio_SA_76_Piano1.wav64");

    mixer_add_event(0, mixer_event, NULL);

    /* Force flashing of denomralized numbers */
    uint32_t fcr31 = C1_FCR31();
    fcr31 |= (1<<24);
    C1_WRITE_FCR31(fcr31);

    bool snow = false;

    ugfx_viewport_t viewport;
    my_ugfx_viewport_init(&viewport, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    data_cache_hit_writeback(&viewport, sizeof(viewport));

    /* Initalize bullet physics stuff */
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	btAlignedObjectArray<btCollisionShape*> collisionShapes;
    dynamicsWorld->setGravity(btVector3(0, -20, 0));

    PhysMesh level_mesh(test_level_2_verts, test_level_2_verts_length, dynamicsWorld, btVector3(0,-2,0), 3, 2);
    PhysMesh snow_mesh(snow_verts, snow_verts_length, dynamicsWorld, btVector3(0,-2,0), 3, 2);

    snow_mesh.body->setCollisionFlags(snow_mesh.body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

    btRigidBody *cube_body;
    btCollisionShape* cube_col;
	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
		cube_col = new btBoxShape(btVector3(1,1,1));
		collisionShapes.push_back(cube_col);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			cube_col->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(2, 10, 0));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, cube_col, localInertia);
		cube_body = new btRigidBody(rbInfo);

		dynamicsWorld->addRigidBody(cube_body);
	}

    glm::vec3 start_pos = glm::vec3(0.0f, 0.0f, -3.0f);
    CharacterController character(dynamicsWorld, start_pos);

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

    cube2.position = glm::vec3(0.0f, 1.0f, -3.0f);
    cube2.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

    cube_model.initalize();
    cube2.initalize();

    float near_plane = 0.1f;
    float far_plane = 100.0f;

    ugfx_matrix_t pv_matrix;
    auto mat = glm::perspective(glm::radians(90.0f), 320.0f/240.0f, near_plane, far_plane);

    uint16_t perspective_normalization_scale = float_to_fixed(get_persp_norm_scale(near_plane, far_plane), 16);

    uint32_t prev_time = (uint32_t)get_ticks();
    float dt = 0.16f;
    controller_data data;
    while(1)
    {
        /* Play audio */
        if(audio_can_write())
        {
            short *buf = audio_write_begin();
            mixer_poll(buf, audio_get_buffer_length());
            audio_write_end();
        }

        controller_read(&data);

        character.update(data);

        if(data.c[0].B)
        {
            snow = !snow;
            if(snow)
            {
                snow_mesh.body->setCollisionFlags(snow_mesh.body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
            }
            else
            {
                snow_mesh.body->setCollisionFlags(snow_mesh.body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
            }
        }

        dynamicsWorld->stepSimulation(1.0f / 30.f, 10);

        btTransform trans = cube_body->getWorldTransform();

        cube2.position = glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ());
        auto rot = trans.getRotation();
        cube2.rotation = glm::quat(rot[3], rot[0], rot[1], rot[2]);

        btTransform player_t = character.body->getWorldTransform();
        btVector3 player_p = player_t.getOrigin();
        rot = player_t.getRotation();
        cube_model.position = glm::vec3(player_p[0], player_p[1], player_p[2]);
        cube_model.rotation = glm::quat(rot[3], rot[0], rot[1], rot[2]);

        glm::mat4 view_mat = character.getViewMatrix();
        glm::mat4 pv = mat * view_mat;
        ugfx_matrix_from_column_major(&pv_matrix, &pv[0][0]);
        data_cache_hit_writeback(&pv_matrix, sizeof(pv_matrix));

        btTransform level_transform = level_mesh.body->getWorldTransform();
        btVector3 level_vec = level_transform.getOrigin();

        glm::mat4 level_mat = glm::translate(glm::mat4(1.0f), glm::vec3(level_vec.getX(), level_vec.getY(), level_vec.getZ()));
        ugfx_matrix_t level_mat_u;
        ugfx_matrix_from_column_major(&level_mat_u, &level_mat[0][0]);
        data_cache_hit_writeback(&level_mat_u, sizeof(level_mat_u));


        while( !(disp = display_lock()) );

        disp_commands.clear();

        disp_commands.push_back(ugfx_set_scissor(0,0, (DISPLAY_WIDTH-1) << 2, (DISPLAY_HEIGHT-1) << 2, UGFX_SCISSOR_DEFAULT));
        disp_commands.push_back(ugfx_load_viewport(0, &viewport));
        disp_commands.push_back(ugfx_set_z_image(0, depth_buffer));
        disp_commands.push_back(ugfx_set_other_modes(UGFX_CYCLE_FILL));
        disp_commands.push_back(ugfx_set_color_image(0, depth_buffer, UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, DISPLAY_WIDTH-1));
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
        disp_commands.push_back(ugfx_set_cull_mode(UGFX_CULL_BACK));
        disp_commands.push_back(ugfx_set_geometry_mode( UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_SMOOTH));
        disp_commands.push_back(ugfx_set_prim_color(0, 0, PACK_RGBA32(255, 0, 0, 255)));
        disp_commands.push_back(ugfx_set_clip_ratio(2));

        cube_model.draw(disp_commands);

        disp_commands.push_back(ugfx_set_prim_color(0, 0, PACK_RGBA32(0, 255, 0, 255)));

        cube2.draw(disp_commands);

        disp_commands.push_back(ugfx_set_model_matrix(0, &level_mat_u));
        disp_commands.push_back(ugfx_push_commands(0, test_level_2_commands, test_level_2_commands_length));

        if(snow)
            disp_commands.push_back(ugfx_push_commands(0, snow_commands, snow_commands_length));

        disp_commands.push_back(ugfx_sync_full());
        disp_commands.push_back(ugfx_finalize());

        data_cache_hit_writeback(&disp_commands[0], disp_commands.size()*sizeof(ugfx_command_t));
        uint32_t num_commands = disp_commands.size();

        dp_sync_status = false;
        rsp_wait();
        ugfx_load(&disp_commands[0], num_commands);
        rsp_run();

        while(!dp_sync_status) {}


        char buf[128];
#if 1
        sprintf(buf, "rotation = %f\n", cube_model.rotation.y);
        graphics_draw_text(disp, 20, 20, buf);
#endif

        sprintf(buf, "DT %f, FPS %f", dt ,1.0f / dt);
        graphics_draw_text(disp, 20, 30, buf);

        /* Play audio again before we wait for vsync */
        if(audio_can_write())
        {
            short *buf = audio_write_begin();
            mixer_poll(buf, audio_get_buffer_length());
            audio_write_end();
        }

        display_show(disp);

        uint32_t new_time = (uint32_t)get_ticks();
        uint32_t ticks_delta;
        if(new_time < prev_time)
        {
            ticks_delta = (0xFFFFFFFF - prev_time) + new_time;
        }
        else
        {
            ticks_delta = new_time - prev_time;
        }
        dt = (float)(ticks_delta) / (float)(TICKS_PER_SECOND);
        prev_time = new_time;
    }
}
