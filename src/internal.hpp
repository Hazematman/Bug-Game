#ifndef INTERNAL_HPP
#define INTERNAL_HPP
#include <libdragon.h>
#include <etl/vector.h>

#define UGFX_CMDBUF_SIZE 256
typedef etl::vector<ugfx_command_t, UGFX_CMDBUF_SIZE> UgfxCommandBuffer;

#endif
