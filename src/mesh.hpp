#ifndef MESH_HPP
#define MESH_HPP
#include <libdragon.h>

#if 0
#define make_vertex_n(vx, vy, vz, vs, vt, nx, ny, nz, va) \
{ \
    .x = float_to_fixed((vx), 5), \
    .y = float_to_fixed((vy), 5), \
    .z = float_to_fixed((vz), 5), \
    .s = float_to_fixed((vs * 32), 6), \
    .t = float_to_fixed((vt * 32), 6), \
    { \
        {}, \
        { \
        .x = (nx), \
        .y = (ny), \
        .z = (nz), \
        .a = (va), \
        }, \
    }, \
}
#endif
constexpr ugfx_vertex_t make_vertex_n(
        float vx, float vy, float vz, 
        float vs, float vt, int nx, int ny, int nz, int va)
{
    ugfx_vertex_t out = {0};
    out.x = float_to_fixed(vx, 5);
    out.y = float_to_fixed(vy, 5);
    out.z = float_to_fixed(vz, 5);
    out.s = float_to_fixed((vs*32), 6);
    out.t = float_to_fixed((vt*32), 6);
    out.attr.normal.x = nx;
    out.attr.normal.y = ny;
    out.attr.normal.z = nz;
    out.attr.normal.a = va;
    return out;
}

#define make_vertex_c(vx, vy, vz, vs, vt, vr, vg, vb, va) \
{ \
    .x = float_to_fixed((vx), 5), \
    .y = float_to_fixed((vy), 5), \
    .z = float_to_fixed((vz), 5), \
    .s = float_to_fixed((vs * 32), 6), \
    .t = float_to_fixed((vt * 32), 6), \
    .attr.color.r = (vr), \
    .attr.color.g = (vg), \
    .attr.color.b = (vb), \
    .attr.color.a = (va), \
}

#endif
