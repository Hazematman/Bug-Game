#ifndef MESH_HPP
#define MESH_HPP
#include <libdragon.h>

constexpr ugfx_vertex_t make_vertex_n(
        float vx, float vy, float vz, 
        float vs, float vt, int nx, int ny, int nz, int va)
{
    ugfx_vertex_t out = {0};
    out.x = float_to_fixed(vx, 5);
    out.y = float_to_fixed(vy, 5);
    out.z = float_to_fixed(vz, 5);
    out.s = float_to_fixed((vs*32.0f), 6);
    out.t = float_to_fixed((vt*32.0f), 6);
    out.attr.normal.x = nx;
    out.attr.normal.y = ny;
    out.attr.normal.z = nz;
    out.attr.normal.a = va;
    return out;
}

constexpr ugfx_vertex_t make_vertex_c(
        float vx, float vy, float vz, 
        float vs, float vt, int vr, int vg, int vb, int va)
{
    ugfx_vertex_t out = {0};
    out.x = float_to_fixed(vx, 5);
    out.y = float_to_fixed(vy, 5);
    out.z = float_to_fixed(vz, 5);
    out.s = float_to_fixed((vs*32.0f), 6);
    out.t = float_to_fixed((vt*32.0f), 6);
    out.attr.color.r = vr;
    out.attr.color.g = vg;
    out.attr.color.b = vb;
    out.attr.color.a = va;
    return out;
}

#endif
