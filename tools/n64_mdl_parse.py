#!/usr/bin/env python3
import sys
import argparse
import os
from PIL import Image

description = "n64_mdl_parse"

S64_VERT_X = 0
S64_VERT_Y = 1
S64_VERT_Z = 2
S64_VERT_XN = 3
S64_VERT_YN = 4
S64_VERT_ZN = 5
S64_VERT_R = 6
S64_VERT_G = 7
S64_VERT_B = 8
S64_VERT_U = 9
S64_VERT_V = 10

def convert(inpu, inlow, inhigh, outlow, outhigh):
    return int(((inpu-inlow) / (inhigh-inlow)) * (outhigh - outlow) + outlow)

def parse_file(file):
    in_model = False
    in_verts = False
    in_faces = False
    verts = []
    faces = {}
    for line in file:
        line_data = line.strip().split()
        if in_model == False and len(line_data) >= 2 and line_data[0] == "BEGIN" and line_data[1] == "MESH":
            in_model = True
        elif in_model == True and len(line_data) >= 2 and line_data[0] == "BEGIN" and line_data[1] == "VERTICES":
            in_verts = True
        elif in_model == True and in_verts == True and len(line_data) >= 2 and line_data[0] == "END" and line_data[1] == "VERTICES":
            in_verts = False
        elif in_model == True and len(line_data) >= 2 and line_data[0] == "BEGIN" and line_data[1] == "FACES":
            in_faces = True
        elif in_model == True and in_faces == True and len(line_data) >= 2 and line_data[0] == "END" and line_data[1] == "FACES":
            in_faces = False
        elif in_model == True and len(line_data) >= 2 and line_data[0] == "END" and line_data[1] == "MESH":
            in_model = False
        elif in_verts:
            vert = [float(i) for i in line_data]
            verts.append(vert)
        elif in_faces:
            material = line_data[-1]
            face = [int(i) for i in line_data[1:-1]]
            if material not in faces:
                faces[material] = []

            faces[material].append(face)


    return (verts, faces)


def export_model(data, input_file, output_name):
    file_name = os.path.basename(os.path.normpath(output_name))
    path_name = os.path.dirname(input_file)

    # Generate commands for materials
    model_verts = []
    model_display_list = "ugfx_set_address_slot(1, {}_verts),\n".format(file_name)
    model_vert_string = ""
    face_index = 0
    material_strings = ""
    for material in data[1]:
        # Go through all the materials and convert their textures into
        # an N64 compatible format like RGBA_5551
        image = Image.open("{}/{}.png".format(path_name, material))
        width, height = image.size
        material_strings += "const uint16_t {}_{}[]  __attribute__ ((aligned (64))) = \n{{\n".format(file_name, material)
        for y in range(height):
            for x in range(width):
                r,g,b,a = image.getpixel((x,y))
                a = 1 if a > 0 else 1
                r = convert(r, 0, 255, 0, 31)
                g = convert(g, 0, 255, 0, 31)
                b = convert(b, 0, 255, 0, 31)
                val = (r<<11)|(g<<6)|(b<<1)|a
                material_strings += "{},".format(hex(val))
        material_strings += "\n};\n"

        # Add material display list command
        if material == "verts":
            print("VERTEX COLORS NOT SUPPORTED YET")
        else:
            width_percent = int((width / 64.0) * 0xFFFF)
            height_percent = int((height / 64.0) * 0xFFFF)
            model_display_list += "ugfx_sync_load(),\n"
            model_display_list += "ugfx_set_combine_mode(UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1, UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1),\n"
            model_display_list += "ugfx_set_geometry_mode(UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_TEXTURE | UGFX_GEOMETRY_SMOOTH),\n"
            model_display_list += "ugfx_set_texture_image(0, {}_{}, UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, {} - 1),\n".format(file_name, material, width)
            model_display_list += "ugfx_set_tile(UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, (2 * {}) >> 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),\n".format(width)
            model_display_list += "ugfx_load_tile(0 << 2, 0 << 2, ({} - 1) << 2, ({} - 1) << 2, 0),\n".format(width, height)
            model_display_list += "ugfx_set_texture_settings({}, {}, 0, 0),\n".format(hex(width_percent), hex(height_percent))


        for face in data[1][material]:
            for v in face:
                model_verts.append(data[0][v])

            face_offset = face_index % 10
            if (face_offset) == 0:
                model_display_list += "ugfx_load_vertices(1, {}*sizeof(ugfx_vertex_t), 0, 32),\n".format(face_index*3)

            model_display_list += "ugfx_draw_triangle({}, {}, {}),\n".format(face_offset*3, face_offset*3+1, face_offset*3+2)
            face_index += 1

    for vert in model_verts:
        model_vert_string += "make_vertex_c({}, {}, {}, {}, {}, {}, {}, {}, {}),\n".format(
                             vert[S64_VERT_X], vert[S64_VERT_Z], -1*vert[S64_VERT_Y], vert[S64_VERT_U], vert[S64_VERT_V],
                             int(vert[S64_VERT_R]*255), int(vert[S64_VERT_G]*255), int(vert[S64_VERT_B]*255), 255)

    model_display_list += "ugfx_finalize(),"

    output_string = """
#include <ugfx.h>
#include \"mesh.hpp\"
#include \"{mesh}.hpp\"

{materials}

const ugfx_vertex_t {mesh}_verts[] = {{
{verts}
}};
const ugfx_command_t {mesh}_commands[] = {{
{commands}
}};

const uint32_t {mesh}_commands_length = sizeof({mesh}_commands) / sizeof(*{mesh}_commands);""".format(
        materials=material_strings, mesh=file_name, verts=model_vert_string, 
        commands=model_display_list)

    file = open(output_name + ".cpp", "w")
    file.write(output_string)
    file.close()


    header_string = """
#ifndef {header_guard}_H
#define {header_guard}_H
#include <ugfx.h>

extern const ugfx_vertex_t {mesh}_verts[];
extern const ugfx_command_t {mesh}_commands[];
extern const uint32_t {mesh}_commands_length;

#endif""".format(header_guard=file_name.upper(), mesh=file_name)

    file = open(output_name + ".hpp", "w")
    file.write(header_string)
    file.close()

def main():
    parser = argparse.ArgumentParser(description)
    parser.add_argument("input", type=str, help="Sausage64 input file") 
    parser.add_argument("output", type=str, help="output file name")
    args = parser.parse_args()


    file = open(args.input, "r")
    data = parse_file(file)
    file.close()

    export_model(data, args.input, args.output)

    return 0

if __name__ == "__main__":
    sys.exit(main())
