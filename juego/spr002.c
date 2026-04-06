/*
 * spr002.c - Prueba de animacion con spritesheet de spr002.bmp
 *
 * Spritesheet: 5 filas x 3 columnas de frames 32x32
 *   Fila 0: estatico (3 frames)
 *   Fila 1: andar izquierda (3 frames)
 *   Fila 2: andar derecha (3 frames)
 *   Fila 3, 4: no usadas
 *
 * Controles:
 *   P   : mover a la derecha (fila 2)
 *   O   : mover a la izquierda (fila 1)
 *   ESC : salir
 *
 * Compilar:
 *   wcl386 -l=dos4g spr002.c engine.c judas.lib
 */

#include "engine.h"

#define FRAME_W       32
#define FRAME_H       32
#define FRAMES_PER_ROW 3
#define ANIM_SPEED     8    /* ticks por frame (~8 fps a 70 Hz) */
#define MOVE_SPEED     2    /* pixels por tick */

/* Filas del spritesheet */
#define ROW_IDLE   0
#define ROW_LEFT   1
#define ROW_RIGHT  2

/* Scancodes */
#define KEY_O   0x18
#define KEY_P   0x19

int main(void)
{
    BITMAP sheet;
    int    x, y;
    int    frame;
    int    anim_timer;
    int    current_row;
    int    moving;

    engine_init();

    load_bmp("spr002.bmp", &sheet);
    set_palette(sheet.palette);

    x           = (SCREEN_W - FRAME_W) / 2;
    y           = (SCREEN_H - FRAME_H) / 2;
    frame       = 0;
    anim_timer  = 0;
    current_row = ROW_IDLE;
    moving      = 0;

    while (!key_pressed(KEY_ESC))
    {
        moving = 0;

        /* Input */
        if (key_pressed(KEY_P))
        {
            x          += MOVE_SPEED;
            current_row = ROW_RIGHT;
            moving      = 1;
            if (x > SCREEN_W - FRAME_W) x = SCREEN_W - FRAME_W;
        }
        else if (key_pressed(KEY_O))
        {
            x          -= MOVE_SPEED;
            current_row = ROW_LEFT;
            moving      = 1;
            if (x < 0) x = 0;
        }
        else
        {
            current_row = ROW_IDLE;
        }

        /* Animacion */
        if (moving || current_row == ROW_IDLE)
        {
            anim_timer++;
            if (anim_timer >= ANIM_SPEED)
            {
                anim_timer = 0;
                frame      = (frame + 1) % FRAMES_PER_ROW;
            }
        }
        else
        {
            /* Al parar: volver al frame 0 */
            frame      = 0;
            anim_timer = 0;
        }

        /* Render */
        vga_clear(0);
        bmp_draw_tile(&sheet, frame, current_row, FRAME_W, FRAME_H, x, y);
        vga_flip();

        timer_wait(1);
    }

    free_bmp(&sheet);
    engine_shutdown();
    return 0;
}