/* ----------------------------------------------------------------
 * fade.c - Probamos vga_fade_out() y vga_fade_in()
 * ---------------------------------------------------------------- */

#include "engine.h"

int main(void)
{
    BITMAP bmp;

    int s,i;

    engine_init();

    load_bmp("mset.bmp", &bmp);
    set_palette(bmp.palette);
    draw_bitmap(&bmp,
        (SCREEN_W - bmp.width)  >> 1,
        (SCREEN_H - bmp.height) >> 1);

    timer_wait(140);        /* espera ~2 segundos mostrando el bitmap */

    
    vga_fade_out(16, 7);    /* 16 pasos, ~0.1s por paso = ~1.7s total */
    timer_wait(70);          /* pausa en negro ~1 segundo */
    vga_fade_in(16, 7);     /* igual de suave al volver */
    

    free(bmp.data);
    engine_shutdown();
    return 0;
}