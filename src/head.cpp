
#include <ugfx.h>
#include "mesh.hpp"

const uint16_t head_head[]  __attribute__ ((aligned (64))) = 
{
0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0x843,0x843,0xf889,0x843,0x843,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0x843,0x1043,0xf889,0x843,0x843,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0x843,0xf089,0xf889,0xf889,0xf889,0x843,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0x843,0x843,0x843,0x843,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,0xf889,
};



ugfx_vertex_t head_None_verts[] = {
make_vertex_c(0.0, -1.0, -0.0, 0.5391, 0.9671, 0, 1, 255, 255),
make_vertex_c(0.7236, -0.4472, 0.5257, 0.3044, 0.8316, 66, 66, 255, 255),
make_vertex_c(-0.2764, -0.4472, 0.8506, 0.9304, 0.8316, 3, 4, 255, 255),
make_vertex_c(0.7236, -0.4472, 0.5257, 0.3044, 0.8316, 66, 66, 255, 255),
make_vertex_c(0.0, -1.0, -0.0, 0.5391, 0.9671, 0, 1, 255, 255),
make_vertex_c(0.7236, -0.4472, -0.5257, 0.4609, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.0, -1.0, -0.0, 0.5391, 0.9671, 0, 1, 255, 255),
make_vertex_c(-0.2764, -0.4472, 0.8506, 0.9304, 0.8316, 3, 4, 255, 255),
make_vertex_c(-0.8944, -0.4472, -0.0, 0.7739, 0.8316, 3, 3, 255, 255),
make_vertex_c(0.0, -1.0, -0.0, 0.5391, 0.9671, 0, 1, 255, 255),
make_vertex_c(-0.8944, -0.4472, -0.0, 0.7739, 0.8316, 3, 3, 255, 255),
make_vertex_c(-0.2764, -0.4472, -0.8506, 0.6174, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.0, -1.0, -0.0, 0.5391, 0.9671, 0, 1, 255, 255),
make_vertex_c(-0.2764, -0.4472, -0.8506, 0.6174, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.7236, -0.4472, -0.5257, 0.4609, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.7236, -0.4472, 0.5257, 0.3044, 0.8316, 66, 66, 255, 255),
make_vertex_c(0.7236, -0.4472, -0.5257, 0.4609, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.8944, 0.4472, -0.0, 0.3826, 0.696, 0, 1, 255, 255),
make_vertex_c(-0.2764, -0.4472, 0.8506, 0.3516, 0.499, 3, 4, 255, 255),
make_vertex_c(0.7236, -0.4472, 0.5257, 0.8496, 0.499, 66, 66, 255, 255),
make_vertex_c(0.2764, 0.4472, 0.8506, 0.6006, 0.0677, 32, 32, 255, 255),
make_vertex_c(-0.8944, -0.4472, -0.0, 0.7739, 0.8316, 3, 3, 255, 255),
make_vertex_c(-0.2764, -0.4472, 0.8506, 0.9304, 0.8316, 3, 4, 255, 255),
make_vertex_c(-0.7236, 0.4472, 0.5257, 0.8521, 0.696, 1, 3, 255, 255),
make_vertex_c(-0.2764, -0.4472, -0.8506, 0.6174, 0.8316, 0, 1, 255, 255),
make_vertex_c(-0.8944, -0.4472, -0.0, 0.7739, 0.8316, 3, 3, 255, 255),
make_vertex_c(-0.7236, 0.4472, -0.5257, 0.6956, 0.696, 31, 31, 255, 255),
make_vertex_c(0.7236, -0.4472, -0.5257, 0.4609, 0.8316, 0, 1, 255, 255),
make_vertex_c(-0.2764, -0.4472, -0.8506, 0.6174, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.2764, 0.4472, -0.8506, 0.5391, 0.696, 0, 1, 255, 255),
make_vertex_c(0.7236, -0.4472, 0.5257, 0.3044, 0.8316, 66, 66, 255, 255),
make_vertex_c(0.8944, 0.4472, -0.0, 0.3826, 0.696, 0, 1, 255, 255),
make_vertex_c(0.2764, 0.4472, 0.8506, 0.2261, 0.696, 32, 32, 255, 255),
make_vertex_c(-0.2764, -0.4472, 0.8506, 0.3516, 0.499, 3, 4, 255, 255),
make_vertex_c(0.2764, 0.4472, 0.8506, 0.6006, 0.0677, 32, 32, 255, 255),
make_vertex_c(-0.7236, 0.4472, 0.5257, 0.1026, 0.0677, 1, 3, 255, 255),
make_vertex_c(-0.8944, -0.4472, -0.0, 0.7739, 0.8316, 3, 3, 255, 255),
make_vertex_c(-0.7236, 0.4472, 0.5257, 0.8521, 0.696, 1, 3, 255, 255),
make_vertex_c(-0.7236, 0.4472, -0.5257, 0.6956, 0.696, 31, 31, 255, 255),
make_vertex_c(-0.2764, -0.4472, -0.8506, 0.6174, 0.8316, 0, 1, 255, 255),
make_vertex_c(-0.7236, 0.4472, -0.5257, 0.6956, 0.696, 31, 31, 255, 255),
make_vertex_c(0.2764, 0.4472, -0.8506, 0.5391, 0.696, 0, 1, 255, 255),
make_vertex_c(0.7236, -0.4472, -0.5257, 0.4609, 0.8316, 0, 1, 255, 255),
make_vertex_c(0.2764, 0.4472, -0.8506, 0.5391, 0.696, 0, 1, 255, 255),
make_vertex_c(0.8944, 0.4472, -0.0, 0.3826, 0.696, 0, 1, 255, 255),
make_vertex_c(0.2764, 0.4472, 0.8506, 0.2261, 0.696, 32, 32, 255, 255),
make_vertex_c(0.8944, 0.4472, -0.0, 0.3826, 0.696, 0, 1, 255, 255),
make_vertex_c(0.0, 1.0, -0.0, 0.4609, 0.5605, 9, 9, 255, 255),
make_vertex_c(-0.7236, 0.4472, 0.5257, 0.8521, 0.696, 1, 3, 255, 255),
make_vertex_c(0.2764, 0.4472, 0.8506, 0.2261, 0.696, 32, 32, 255, 255),
make_vertex_c(0.0, 1.0, -0.0, 0.4609, 0.5605, 9, 9, 255, 255),
make_vertex_c(-0.7236, 0.4472, -0.5257, 0.6956, 0.696, 31, 31, 255, 255),
make_vertex_c(-0.7236, 0.4472, 0.5257, 0.8521, 0.696, 1, 3, 255, 255),
make_vertex_c(0.0, 1.0, -0.0, 0.4609, 0.5605, 9, 9, 255, 255),
make_vertex_c(0.2764, 0.4472, -0.8506, 0.5391, 0.696, 0, 1, 255, 255),
make_vertex_c(-0.7236, 0.4472, -0.5257, 0.6956, 0.696, 31, 31, 255, 255),
make_vertex_c(0.0, 1.0, -0.0, 0.4609, 0.5605, 9, 9, 255, 255),
make_vertex_c(0.8944, 0.4472, -0.0, 0.3826, 0.696, 0, 1, 255, 255),
make_vertex_c(0.2764, 0.4472, -0.8506, 0.5391, 0.696, 0, 1, 255, 255),
make_vertex_c(0.0, 1.0, -0.0, 0.4609, 0.5605, 9, 9, 255, 255),

};
ugfx_command_t head_None_commands[] = {
ugfx_set_address_slot(1, head_None_verts),
ugfx_sync_pipe(),
ugfx_set_combine_mode(UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1, UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1),
ugfx_set_geometry_mode(UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_TEXTURE | UGFX_GEOMETRY_SMOOTH),
ugfx_set_texture_image(0, head_head, UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, 16 - 1),
ugfx_set_tile(UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, (2 * 16) >> 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
ugfx_load_tile(0 << 2, 0 << 2, (16 - 1) << 2, (16 - 1) << 2, 0),
ugfx_set_texture_settings(0x3fff, 0x3fff, 0, 0),
ugfx_load_vertices(1, 0*sizeof(ugfx_vertex_t), 0, 32),
ugfx_draw_triangle(0, 1, 2),
ugfx_draw_triangle(3, 4, 5),
ugfx_draw_triangle(6, 7, 8),
ugfx_draw_triangle(9, 10, 11),
ugfx_draw_triangle(12, 13, 14),
ugfx_draw_triangle(15, 16, 17),
ugfx_draw_triangle(18, 19, 20),
ugfx_draw_triangle(21, 22, 23),
ugfx_draw_triangle(24, 25, 26),
ugfx_draw_triangle(27, 28, 29),
ugfx_load_vertices(1, 30*sizeof(ugfx_vertex_t), 0, 32),
ugfx_draw_triangle(0, 1, 2),
ugfx_draw_triangle(3, 4, 5),
ugfx_draw_triangle(6, 7, 8),
ugfx_draw_triangle(9, 10, 11),
ugfx_draw_triangle(12, 13, 14),
ugfx_draw_triangle(15, 16, 17),
ugfx_draw_triangle(18, 19, 20),
ugfx_draw_triangle(21, 22, 23),
ugfx_draw_triangle(24, 25, 26),
ugfx_draw_triangle(27, 28, 29),
ugfx_finalize(),
};

uint32_t head_None_verts_length = sizeof(head_None_verts) / sizeof(*head_None_verts);
uint32_t head_None_commands_length = sizeof(head_None_commands) / sizeof(*head_None_commands);






AnimationGroup head_groups[] = {
};

MeshDef head_meshes[] ={
{{0.0, 0.0, -0.0}, head_None_verts, head_None_commands, sizeof(head_None_commands) / sizeof(*head_None_commands)},
};

ModelDef head_def = {
head_meshes,
sizeof(head_meshes) / sizeof(*head_meshes),
head_groups,
sizeof(head_groups) / sizeof(*head_groups),
-0.8944, 0.8944, -1.0, 1.0, -0.8506, 0.8506,
};

