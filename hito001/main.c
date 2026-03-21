/*
 * main.c - Demo basico: sprite con gravedad y colision con el suelo
 *
 * Controles:
 *   Izquierda / Derecha : mover
 *   ESPACIO             : saltar
 *   ESC                 : salir
 *
 * Compilar:
 *   wcl386 -l=dos4g main.c engine.c
 */

#include "engine.h"

#define PLAYER_W   16
#define PLAYER_H   16

#define GRAVITY        0.4f
#define JUMP_FORCE    -6.0f
#define MOVE_SPEED     1.2f
#define GROUND_Y      (SCREEN_H - PLAYER_H - 1)

/* Sprite 16x16: color 14=amarillo, 4=rojo, 0=transparente */
static const unsigned char player_sprite[PLAYER_W * PLAYER_H] = {
    0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
    0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
   14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
   14,14, 4, 4,14,14,14,14,14,14,14,14, 4, 4,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
   14,14,14,14, 4,14,14,14,14,14,14, 4,14,14,14,14,
   14,14,14,14,14, 4, 4, 4, 4, 4, 4,14,14,14,14,14,
   14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
    0,14,14,14,14,14,14,14,14,14,14,14,14,14,14, 0,
    0, 0,14,14,14,14,14,14,14,14,14,14,14,14, 0, 0,
    0, 0, 6, 6, 6, 6, 0, 0, 0, 0, 6, 6, 6, 6, 0, 0,
    0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
    0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
    0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
    0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
    0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0,
};

typedef struct {
    float x, y;
    float vel_y;
    int   on_ground;
} Player;

static void player_init(Player *p)
{
    p->x        = SCREEN_W / 2 - PLAYER_W / 2;
    p->y        = GROUND_Y;
    p->vel_y    = 0.0f;
    p->on_ground = 1;
}

static void player_update(Player *p)
{
    if (key_pressed(KEY_LEFT))  p->x -= MOVE_SPEED;
    if (key_pressed(KEY_RIGHT)) p->x += MOVE_SPEED;

    if (key_pressed(KEY_SPACE) && p->on_ground)
    {
        p->vel_y    = JUMP_FORCE;
        p->on_ground = 0;
    }

    p->vel_y += GRAVITY;
    p->y     += p->vel_y;

    if (p->y >= GROUND_Y)
    {
        p->y        = GROUND_Y;
        p->vel_y    = 0.0f;
        p->on_ground = 1;
    }

    if (p->x < 0)                    p->x = 0;
    if (p->x > SCREEN_W - PLAYER_W)  p->x = SCREEN_W - PLAYER_W;
}

static void player_draw(const Player *p)
{
    draw_sprite(player_sprite, (int)p->x, (int)p->y, PLAYER_W, PLAYER_H);
}

int main(void)
{
    Player player;
    int    i;

    vga_set_mode13h();
    keyboard_install();
    timer_install();

    player_init(&player);

    while (!key_pressed(KEY_ESC))
    {
        player_update(&player);

        vga_clear(1);

        /* Suelo */
        for (i = 0; i < SCREEN_W; i++)
            vga_put_pixel(i, GROUND_Y + PLAYER_H, 2);

        player_draw(&player);

        vga_flip();
        timer_wait(1);
    }

    timer_uninstall();
    keyboard_uninstall();
    vga_set_text_mode();
    return 0;
}