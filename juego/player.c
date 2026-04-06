/*
 * player.c - Sistema de Eric (jugador) en Tempus Fugit
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "engine.h"
#include "game.h"
#include "screen.h"
#include "player.h"





/* Spritesheet global de Eric, cargado en screen.c */
extern BITMAP g_spritesheet;

/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void player_init(void)
{
    g_game.player.x          = 20.0f;
    g_game.player.y          = 100.0f;
    g_game.player.vel_x      = 0.0f;
    g_game.player.vel_y      = 0.0f;
    g_game.player.on_ground  = 0;
    g_game.player.facing     = 1;
    g_game.player.anim_frame = 0;
    g_game.player.anim_timer = 0;
    g_game.player.lives      = PLAYER_LIVES;
}

/* ----------------------------------------------------------------
 * RESOLVE COLLISIONS
 * Resuelve colisiones AABB con las plataformas de la pantalla
 * ---------------------------------------------------------------- */
static void resolve_collisions(void)
{
    int i;
    int px, py;
    int ov_top, ov_bottom, ov_left, ov_right;
    int min, axis;
    Platform *plat;

    g_game.player.on_ground = 0;

    for (i = 0; i < g_screen_data.platform_count; i++)
    {
        plat = &g_screen_data.platforms[i];
        px   = (int)g_game.player.x;
        py   = (int)g_game.player.y;

        if (px + PLAYER_WIDTH <= plat->x) continue;
        if (px >= plat->x + plat->w)  continue;
        if (py + PLAYER_HEIGHT <= plat->y) continue;
        if (py >= plat->y + plat->h)  continue;

        ov_top    = (py + PLAYER_HEIGHT) - plat->y;
        ov_bottom = (plat->y + plat->h) - py;
        ov_left   = (px + PLAYER_WIDTH) - plat->x;
        ov_right  = (plat->x + plat->w) - px;

        min = ov_top; axis = 0;
        if (ov_bottom < min) { min = ov_bottom; axis = 1; }
        if (ov_left   < min) { min = ov_left;   axis = 2; }
        if (ov_right  < min) { min = ov_right;  axis = 3; }

        switch (axis)
        {
            case 0:
                g_game.player.y         = (float)(plat->y - PLAYER_HEIGHT);
                g_game.player.vel_y     = 0.0f;
                g_game.player.on_ground = 1;
                break;
            case 1:
                g_game.player.y     = (float)(plat->y + plat->h);
                g_game.player.vel_y = 0.0f;
                break;
            case 2:
                g_game.player.x     = (float)(plat->x - PLAYER_WIDTH);
                g_game.player.vel_x = 0.0f;
                break;
            case 3:
                g_game.player.x     = (float)(plat->x + plat->w);
                g_game.player.vel_x = 0.0f;
                break;
        }
    }
}

/* ----------------------------------------------------------------
 * UPDATE
 * Devuelve PLAYER_* segun el evento ocurrido
 * ---------------------------------------------------------------- */
int player_update(void)
{
    int moving;

    moving = 0;

    if (key_pressed(KEY_LEFT))
    {
        g_game.player.vel_x  = -PLAYER_SPEED;
        g_game.player.facing = -1;
        moving               = 1;
    }
    else if (key_pressed(KEY_RIGHT))
    {
        g_game.player.vel_x  = PLAYER_SPEED;
        g_game.player.facing = 1;
        moving               = 1;
    }
    else
    {
        g_game.player.vel_x = 0.0f;
    }

    if (key_pressed(KEY_SPACE) && g_game.player.on_ground)
    {
        g_game.player.vel_y     = PLAYER_JUMP;
        g_game.player.on_ground = 0;
        sfx_play(SFX_JUMP, 64, MIDDLE);
    }

    g_game.player.vel_y += PLAYER_GRAVITY;
    g_game.player.x     += g_game.player.vel_x;
    g_game.player.y     += g_game.player.vel_y;

    resolve_collisions();

    if (g_game.player.on_ground && moving)
    {
        if (++g_game.player.anim_timer >= ANIM_SPEED)
        {
            g_game.player.anim_timer = 0;
            g_game.player.anim_frame =
                (g_game.player.anim_frame + 1) % FRAMES_PER_ROW;
        }
    }
    else
    {
        g_game.player.anim_frame = 0;
        g_game.player.anim_timer = 0;
    }

    /* Comprobar si Eric ha salido por algun borde */
    if (g_game.player.x < 0)                    return PLAYER_LEFT;
    if (g_game.player.x > SCREEN_W - PLAYER_WIDTH)  return PLAYER_RIGHT;
    if (g_game.player.y < 0)                     return PLAYER_UP;
    if (g_game.player.y > SCREEN_H)              return PLAYER_DEAD;

    return PLAYER_NONE;
}

/* ----------------------------------------------------------------
 * DRAW
 * ---------------------------------------------------------------- */
void player_draw(void)
{
    int frame;
    int sx, sy, dx, dy;
    unsigned char c;

    frame = (!g_game.player.on_ground) ? FRAME_JUMP :
             g_game.player.anim_frame;

    if (g_game.player.facing == 1)
    {
        bmp_draw_tile(&g_spritesheet, frame, 0,
                      PLAYER_WIDTH, PLAYER_HEIGHT,
                      (int)g_game.player.x,
                      (int)g_game.player.y);
    }
    else
    {
        for (sy = 0; sy < PLAYER_HEIGHT; sy++)
        {
            dy = (int)g_game.player.y + sy;
            if (dy < 0 || dy >= SCREEN_H) continue;
            for (sx = 0; sx < PLAYER_WIDTH; sx++)
            {
                dx = (int)g_game.player.x + (PLAYER_WIDTH - 1 - sx);
                if (dx < 0 || dx >= SCREEN_W) continue;
                c = g_spritesheet.data[sy * g_spritesheet.width +
                    frame * PLAYER_WIDTH + sx];
                if (c == 0) continue;
                back_buffer[dy * SCREEN_W + dx] = c;
            }
        }
    }
}

/* ----------------------------------------------------------------
 * HIT
 * Eric recibe un impacto: pierde una vida y vuelve al inicio
 * ---------------------------------------------------------------- */
void player_hit(void)
{
    g_game.player.lives--;
    player_place(20, 140);
}

/* ----------------------------------------------------------------
 * PLACE
 * Reposiciona a Eric en unas coordenadas
 * ---------------------------------------------------------------- */
void player_place(int x, int y)
{
    g_game.player.x     = (float)x;
    g_game.player.y     = (float)y;
    g_game.player.vel_x = 0.0f;
    g_game.player.vel_y = 0.0f;
}
