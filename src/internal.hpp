#ifndef INTERNAL_HPP
#define INTERNAL_HPP
#include <libdragon.h>
#include <etl/vector.h>

class CharacterController;

#define UNUSED(x) (void)(x)

#define UGFX_CMDBUF_SIZE 1024
typedef etl::vector<ugfx_command_t, UGFX_CMDBUF_SIZE> UgfxCommandBuffer;

extern CharacterController *global_character;

#endif
