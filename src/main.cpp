#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <libdragon.h>

static resolution_t res = RESOLUTION_320x240;
static bitdepth_t bit = DEPTH_32_BPP;


static display_context_t disp = 0;

int main(void)
{
    init_interrupts();

    /* Initalize basics */
    display_init(res, bit, 2, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    ugfx_init(UGFX_DEFAULT_RDP_BUFFER_SIZE);
    dfs_init(DFS_DEFAULT_LOCATION);

    char str[128];
    int count = 0;

    while(1)
    {
        sprintf(str, "Hello %d\n", count);
        while( !(disp = display_lock()) );
        graphics_fill_screen(disp, graphics_make_color(0,0,255,255));
        graphics_draw_text(disp, 10, 10, str); 
        display_show(disp);

        count++;
    }
}
