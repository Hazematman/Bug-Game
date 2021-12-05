#!/usr/bin/env python3
import sys
import argparse
import os
import math
from PIL import Image
from scipy.spatial.transform import Rotation as scirot

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

def min(x, y):
    return x if x < y else y

def max(x, y):
    return x if x > y else y

def convert(inpu, inlow, inhigh, outlow, outhigh):
    return int(((inpu-inlow) / (inhigh-inlow)) * (outhigh - outlow) + outlow)

def parse_file(file):
    in_model = False
    in_verts = False
    in_faces = False
    in_animation = False
    in_keyframe = False
    root = [0,0,0]
    verts = []
    faces = {}
    frame = []
    animations = {}
    mesh_name = ""
    meshes = {}
    current_anim = ""
    for line in file:
        line_data = line.strip().split()
        if in_model == False and len(line_data) >= 2 and line_data[0] == "BEGIN" and line_data[1] == "MESH":
            in_model = True
            mesh_name = line_data[2]
            root = [0,0,0]
            verts = []
            faces = {}
            meshes[mesh_name] = [root, verts, faces]
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
        elif in_model == False and len(line_data) >= 2 and line_data[0] == "BEGIN" and line_data[1] == "ANIMATION":
            in_animation = True
            if line_data[2] not in animations:
                animations[line_data[2]] = []

            current_anim = line_data[2]
        elif in_keyframe == True and len(line_data) >= 2 and line_data[0] == "END" and line_data[1] == "KEYFRAME":
            in_keyframe = False
        elif in_animation == True and len(line_data) >= 2 and line_data[0] == "END" and line_data[1] == "ANIMATION":
            in_animation = False
        elif in_animation == True and len(line_data) >= 2 and line_data[0] == "BEGIN" and line_data[1] == "KEYFRAME":
            in_keyframe = True
            frame = [int(line_data[2])]
            animations[current_anim].append(frame)
        elif in_model == True and in_faces == False and in_verts == False and len(line_data) >= 2 and line_data[0] == "ROOT":
            root[0] = float(line_data[1])
            root[1] = float(line_data[2])
            root[2] = float(line_data[3])
        elif in_verts:
            vert = [float(i) for i in line_data]
            verts.append(vert)
        elif in_faces:
            material = line_data[-1]
            face = [int(i) for i in line_data[1:-1]]
            if material not in faces:
                faces[material] = []

            faces[material].append(face)
        elif in_keyframe == True:
            transform = [float(i) for i in line_data[1:]]
            value = [line_data[0]] + transform 
            frame.append(value)


    return [meshes, animations]


def export_model(model_data, input_file, output_name):
    file_name = os.path.basename(os.path.normpath(output_name))
    path_name = os.path.dirname(input_file)

    # TODO find optimal order to store verts in
    # so we have to do the minimal amount of load verts calls
    
    meshes = model_data[0]
    meshes_output_string = ""
    material_strings = ""
    materials = []
    mesh_def_strings = "MeshDef {}_meshes[] ={{\n".format(file_name)
    min_x = 1*1000
    min_y = 1*1000
    min_z = 1*1000
    max_x = -1*1000
    max_y = -1*1000
    max_z = -1*1000
    for mesh in meshes:
        data = meshes[mesh][1:]
        mesh_name = mesh.replace(".", "_")
        # Generate commands for materials
        model_verts = []
        model_display_list = "ugfx_set_address_slot(1, {}_{}_verts),\n".format(file_name, mesh_name)
        model_vert_string = ""
        face_index = 0
        for material in data[1]:
            material_name = material.replace(".", "_")
            if material != "verts" and material not in materials:
                # Go through all the materials and convert their textures into
                # an N64 compatible format like RGBA_5551
                image = Image.open("{}/{}.png".format(path_name, material))
                image = image.convert("RGBA")
                width, height = image.size
                material_strings += "const uint16_t {}_{}[]  __attribute__ ((aligned (64))) = \n{{\n".format(file_name, material_name)
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
                materials.append(material)

            # Add material display list command
            if material == "verts":
                model_display_list += "ugfx_sync_pipe(),\n"
                model_display_list += "ugfx_set_combine_mode(UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_SHADE_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1, UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_SHADE_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1),\n"
                model_display_list += "ugfx_set_geometry_mode(UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_SMOOTH),\n"
            else:
                width_percent = int((width / 64.0) * 0xFFFF)
                height_percent = int((height / 64.0) * 0xFFFF)
                model_display_list += "ugfx_sync_pipe(),\n"
                model_display_list += "ugfx_set_combine_mode(UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1, UGFX_CC_SUB_0, UGFX_CC_SUB_0, UGFX_CC_MUL_0, UGFX_CC_T0_COLOR, UGFX_AC_0, UGFX_AC_0, UGFX_AC_0, UGFX_AC_1),\n"
                model_display_list += "ugfx_set_geometry_mode(UGFX_GEOMETRY_SHADE | UGFX_GEOMETRY_ZBUFFER | UGFX_GEOMETRY_TEXTURE | UGFX_GEOMETRY_SMOOTH),\n"
                model_display_list += "ugfx_set_texture_image(0, {}_{}, UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, {} - 1),\n".format(file_name, material_name, width)
                model_display_list += "ugfx_set_tile(UGFX_FORMAT_RGBA, UGFX_PIXEL_SIZE_16B, (2 * {}) >> 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),\n".format(width)
                model_display_list += "ugfx_load_tile(0 << 2, 0 << 2, ({} - 1) << 2, ({} - 1) << 2, 0),\n".format(width, height)
                model_display_list += "ugfx_set_texture_settings({}, {}, 0, 0),\n".format(hex(width_percent), hex(height_percent))

            for face in data[1][material]:
                for v in face:
                    model_verts.append(data[0][v][:])

                face_offset = face_index % 10
                if (face_offset) == 0:
                    model_display_list += "ugfx_load_vertices(1, {}*sizeof(ugfx_vertex_t), 0, 32),\n".format(face_index*3)

                model_display_list += "ugfx_draw_triangle({}, {}, {}),\n".format(face_offset*3, face_offset*3+1, face_offset*3+2)
                face_index += 1


        mesh_def_strings += "{{{{{x}, {y}, {z}}}, {file}_{mesh}_verts, {file}_{mesh}_commands, sizeof({file}_{mesh}_commands) / sizeof(*{file}_{mesh}_commands)}},\n".format(
                            file=file_name, mesh=mesh_name, 
                            x=meshes[mesh][0][0], y=meshes[mesh][0][2], z=-1*meshes[mesh][0][1])



        for vert in model_verts:
            min_x = min(min_x, vert[S64_VERT_X])
            max_x = max(max_x, vert[S64_VERT_X])
            min_y = min(min_y, vert[S64_VERT_Z])
            max_y = max(max_y, vert[S64_VERT_Z])
            min_z = min(min_z, -1*vert[S64_VERT_Y])
            max_z = max(max_z, -1*vert[S64_VERT_Y])
            model_vert_string += "make_vertex_c({}, {}, {}, {}, {}, {}, {}, {}, {}),\n".format(
                                 vert[S64_VERT_X], vert[S64_VERT_Z], -1*vert[S64_VERT_Y], 
                                 vert[S64_VERT_U], vert[S64_VERT_V],
                                 int(vert[S64_VERT_R]*255), int(vert[S64_VERT_G]*255), 
                                 int(vert[S64_VERT_B]*255), 255)

        model_display_list += "ugfx_finalize(),"

        meshes_output_string += """
ugfx_vertex_t {file}_{mesh}_verts[] = {{
{verts}
}};
ugfx_command_t {file}_{mesh}_commands[] = {{
{commands}
}};

uint32_t {file}_{mesh}_verts_length = sizeof({file}_{mesh}_verts) / sizeof(*{file}_{mesh}_verts);
uint32_t {file}_{mesh}_commands_length = sizeof({file}_{mesh}_commands) / sizeof(*{file}_{mesh}_commands);
""".format(
        file=file_name, mesh=mesh_name, verts=model_vert_string, 
        commands=model_display_list)

    mesh_def_strings += "};"
    animations = model_data[1]
    animation_group_string = "AnimationGroup {file}_groups[] = {{\n".format(file=file_name)
    frame_strings = ""
    animationTrans_string = ""
    for animation in animations:
        animation_string = ""
        frame_strings += "Animation {file}_{animation}[] = {{".format(file=file_name, animation=animation)
        animation_group_string += "{{\"{animation}\", {file}_{animation}, sizeof({file}_{animation})/sizeof(*{file}_{animation})}},\n".format(file=file_name, 
                                  animation=animation)
        for frame in animations[animation]:
            animation_name = "{file}_{animation}_{keyframe}_trans".format(file=file_name, animation=animation, keyframe=frame[0])
            animationTrans_string += "AnimationTrans {}[] = {{".format(animation_name)
            for trans in frame[1:]:
                x_rot = scirot.from_euler('x', trans[4])
                y_rot = scirot.from_euler('y', trans[5])
                z_rot = scirot.from_euler('z', trans[6])
                # Calculate rotation quaternion
                #original_rotation = x_rot * y_rot * z_rot
                original_rotation = scirot.from_euler('xyz', [trans[4], trans[5], trans[6]])
                new_rot = scirot.from_euler('X', -90, degrees=True) * original_rotation * scirot.from_euler('X', 90, degrees=True)
                quat = new_rot.as_quat()
                animationTrans_string += "{{{},{},{},{},{},{},{},{},{},{}}},".format(
                                                                                  trans[1], trans[3], -trans[2],
                                                                                  quat[0], quat[1], quat[2], quat[3],
                                                                                  trans[7], trans[9], trans[8])
            animationTrans_string += "};\n"
            frame_strings += "{{{keyframe}, {trans}}},".format(keyframe=frame[0], trans=animation_name)
        frame_strings += "};"

    animation_group_string += "};"


    output_string = """
#include <ugfx.h>
#include \"mesh.hpp\"

{materials}

{meshes}

{animation_trans}

{animations}

{animation_group}

{mesh_def}

ModelDef {mesh}_def = {{
{mesh}_meshes,
sizeof({mesh}_meshes) / sizeof(*{mesh}_meshes),
{mesh}_groups,
sizeof({mesh}_groups) / sizeof(*{mesh}_groups),
{min_x}, {max_x}, {min_y}, {max_y}, {min_z}, {max_z},
}};

""".format(
        mesh=file_name, materials=material_strings, meshes=meshes_output_string, 
        animations=frame_strings, animation_group=animation_group_string, mesh_def=mesh_def_strings,
        animation_trans=animationTrans_string,
        min_x=min_x, max_x=max_x, min_y=min_y, max_y=max_y, min_z=min_z, max_z=max_z)
    file = open(output_name + ".cpp", "w")
    file.write(output_string)
    file.close()
    return

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
