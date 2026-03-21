

 /* ----------------------------------------------------------------------------
 * sprite.c - Prueba de animacion con spritesheet
 *
 * Mueve un sprite de 2 frames horizontalmente
 * alternando frames cada 0.5 segundos (~35 ticks a 70 Hz)
 *
 * Necesita en el mismo directorio:
 *   - spr001.bmp  (spritesheet 32x16, 2 frames de 16x16)
 * ---------------------------------------------------------------------------- */

#include "engine.h"

#define FRAME_W      16
#define FRAME_H      16
#define FRAME_COUNT   2
#define ANIM_SPEED   35    /* ticks por frame (~0.5s a 70 Hz) */
#define MOVE_SPEED    1    /* pixels por tick                  */

int main(void)
{
    BITMAP sheet;
    int    x, y;
    int    frame;
    int    anim_timer;
    int    dx;             /* direccion: 1=derecha, -1=izquierda */

    engine_init();

    load_bmp("spr001.bmp", &sheet);
    set_palette(sheet.palette);

    /* Posicion inicial centrada verticalmente */
    x          = 0;
    y          = (SCREEN_H - FRAME_H) / 2;
    frame      = 0;
    anim_timer = 0;
    dx         = 1;

    while (!key_pressed(KEY_ESC))
    {
        /* Mover horizontalmente y rebotar en los bordes */
        x += dx * MOVE_SPEED;
        if (x <= 0)
        {
            x  = 0;
            dx = 1;
        }
        if (x >= SCREEN_W - FRAME_W)
        {
            x  = SCREEN_W - FRAME_W;
            dx = -1;
        }

        /* Avanzar frame de animacion cada ANIM_SPEED ticks */
        anim_timer++;
        if (anim_timer >= ANIM_SPEED)
        {
            anim_timer = 0;
            frame      = (frame + 1) % FRAME_COUNT;
        }

        /* Render */
        vga_clear(0);
        bmp_draw_tile(&sheet, frame, 0, FRAME_W, FRAME_H, x, y);
        vga_flip();

        timer_wait(1);
    }

    free_bmp(&sheet);
    engine_shutdown();
    return 0;
}