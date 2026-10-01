/*
 * player.c - Sistema de Eric (jugador) en Time Daze
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "engine.h"
#include "game.h"
#include "screen.h"
#include "player.h"
#include "dialog.h"


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
    invuln_timer = 0;
}

/* ----------------------------------------------------------------
 * RESOLVE COLLISIONS
 * Resuelve colisiones AABB con las plataformas de la pantalla
 * ---------------------------------------------------------------- */
static void resolve_collisions(void)
{
    int i;
    int px, py;
    Platform *plat;

    g_game.player.on_ground = 0;

    for (i = 0; i < g_screen_data.platform_count; i++)
    {
        plat = &g_screen_data.platforms[i];
        px   = (int)g_game.player.x;
        py   = (int)g_game.player.y;

        /* Solo colision por arriba:                          */
        /* El borde inferior de Eric toca el borde superior   */
        /* de la plataforma, viniendo desde arriba            */
        if (px + PLAYER_WIDTH  <= plat->x) continue;
        if (px >= plat->x + plat->w)       continue;
        if (py + PLAYER_HEIGHT <  plat->y) continue;
        if (py + PLAYER_HEIGHT >  plat->y + plat->h + 4) continue;
        if (g_game.player.vel_y < 0.0f)   continue;

        g_game.player.y         = (float)(plat->y - PLAYER_HEIGHT);
        g_game.player.vel_y     = 0.0f;
        g_game.player.on_ground = 1;
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

    if (!dialog_is_open())
    {
        if (key_pressed(KEY_O))
        {
            g_game.player.vel_x  = -PLAYER_SPEED;
            g_game.player.facing = -1;
            moving               = 1;
        }
        else if (key_pressed(KEY_P))
        {
            g_game.player.vel_x  = PLAYER_SPEED;
            g_game.player.facing = 1;
            moving               = 1;
        }
        else
        {
            g_game.player.vel_x = 0.0f;
        }

        if (key_pressed(KEY_Q) && g_game.player.on_ground)
        {
            g_game.player.vel_y     = PLAYER_JUMP;
            g_game.player.on_ground = 0;
            sfx_play(SFX_JUMP, 64, MIDDLE);
        }
    }
    else
    {
        g_game.player.vel_x = 0.0f;  /* parar a Eric mientras hay dialogo */
    }

    g_game.player.vel_y += PLAYER_GRAVITY;

    /* Limitar velocidad maxima de caida */
    if (g_game.player.vel_y > 8.0f)
        g_game.player.vel_y = 8.0f;
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
    if (g_game.player.x < 0)                        return PLAYER_LEFT;
    if (g_game.player.x > SCREEN_W - PLAYER_WIDTH)  return PLAYER_RIGHT;
    if (g_game.player.y < 0)                        return PLAYER_UP;
    if (g_game.player.y > SCREEN_H)                 return PLAYER_DEAD;

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

    /* Si no hay spritesheet dibujar placeholder */
    if (g_spritesheet.data == NULL)
    {
        dx = (int)g_game.player.x;
        dy = (int)g_game.player.y;
        draw_line(dx,                  dy,
                  dx + PLAYER_WIDTH,   dy,                  14);
        draw_line(dx + PLAYER_WIDTH,   dy,
                  dx + PLAYER_WIDTH,   dy + PLAYER_HEIGHT,  14);
        draw_line(dx + PLAYER_WIDTH,   dy + PLAYER_HEIGHT,
                  dx,                  dy + PLAYER_HEIGHT,  14);
        draw_line(dx,                  dy + PLAYER_HEIGHT,
                  dx,                  dy,                  14);
        return;
    }

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
    int i;

    /* Reproucir sonido de pérdida de vida */
    sfx_play(SFX_HURT, 45, MIDDLE);

    /* Pausa breve trasladando el impacto, troceada en pasos pequenos
     * con sound_update() entre medias -- timer_wait() por si solo no
     * alimenta el mezclador de audio, y una espera larga de un tiron
     * distorsiona la musica de fondo al reanudar. */
    for (i = 0; i < 95; i += 4)
    {
        sound_update();
        timer_wait(4);
    }

    if (!g_debug_infinite_lives)
        g_game.player.lives--;

     /* Caso específico para P3 Prehistoria: el oso bloquea el paso a proposito mientras la miel no este resuelta
     *  (saltarlo es imposible por diseno). Si nso mata el oso en el lado derecho, el puzle quedaria sin resolver
     *  ya que Eric aparecería en el lado derecho. Hacemos que reaparezca siempre en la izquierda. */
    if (g_game.screen.current_epoch == EPOCH_PREHISTORY && g_game.screen.current_screen == 2)
    {
        player_place(20, 100);
    }
    else if (g_game.player.x < SCREEN_W / 2)
        player_place(20, 100);      /* estaba en la izquierda */
    else
        player_place(SCREEN_W - PLAYER_WIDTH - 20, 100); /* Estaba en la derecha */

    g_game.player.invuln_timer = 90;
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
