
#include <ugfx.h>
#include "mesh.hpp"

const uint16_t centi_body_body_top[]  __attribute__ ((aligned (64))) = 
{
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc81f,0xc81f,0xc81f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
};



ugfx_vertex_t centi_body_None_verts[] = {
make_vertex_c(1.0, 0.0, -0.0, 1.0, 1.0, 255, 255, 255, 255),
make_vertex_c(-1.0, 2.0, -0.0, 0.0, 0.0, 255, 255, 255, 255),
make_vertex_c(-1.0, 0.0, -0.0, 0.0, 1.0, 255, 255, 255, 255),
make_vertex_c(1.0, 0.0, -0.0, 1.0, 1.0, 255, 255, 255, 255),
make_vertex_c(1.0, 2.0, -0.0, 1.0, 0.0, 255, 255, 255, 255),
make_vertex_c(-1.0, 2.0, -0.0, 0.0, 0.0, 255, 255, 255, 255),

};
ugfx_command_t centi_body_None_commands[] = {
ugfx_set_address_slot(1, centi_body_None_verts),
ugfx_sync_pipe(),
ugfx_set_combine_mode(UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_T0_ALPHA, UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_T0_ALPHA),
ugfx_set_geometry_mode(UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_TEXTURE | UGFX_GEOMETRY_SMOOTH),
ugfx_set_texture_image(0, centi_body_body_top, UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, 16 - 1),
ugfx_set_tile(UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, (2 * 16) >> 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
ugfx_load_tile(0 << 2, 0 << 2, (16 - 1) << 2, (16 - 1) << 2, 0),
ugfx_set_texture_settings(0x3fff, 0x3fff, 0, 0),
ugfx_load_vertices(1, 0*sizeof(ugfx_vertex_t), 0, 32),
ugfx_draw_triangle(0, 1, 2),
ugfx_draw_triangle(3, 4, 5),
ugfx_finalize(),
};

uint32_t centi_body_None_verts_length = sizeof(centi_body_None_verts) / sizeof(*centi_body_None_verts);
uint32_t centi_body_None_commands_length = sizeof(centi_body_None_commands) / sizeof(*centi_body_None_commands);




Animation centi_body_body_bottomAction[] = {};

AnimationGroup centi_body_groups[] = {
};

MeshDef centi_body_meshes[] ={
{{0.0, 0.0, -0.0}, centi_body_None_verts, centi_body_None_commands, sizeof(centi_body_None_commands) / sizeof(*centi_body_None_commands)},
};

ModelDef centi_body_def = {
centi_body_meshes,
sizeof(centi_body_meshes) / sizeof(*centi_body_meshes),
centi_body_groups,
sizeof(centi_body_groups) / sizeof(*centi_body_groups),
-1.0, 1.0, 0.0, 2.0, -0.0, -0.0,
};

