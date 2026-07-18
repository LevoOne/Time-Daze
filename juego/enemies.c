/*
 * enemies.c - Sistema de enemigos de Tempus Fugit
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "engine.h"
#include "game.h"
#include "enemies.h"
#include "player.h"
#include "puzzles.h"
#include "screen.h"
#include "inventory.h"





/* ----------------------------------------------------------------
 * VARIABLE GLOBAL
 * ---------------------------------------------------------------- */
EnemyList g_enemies;

/* ----------------------------------------------------------------
 * MACRO FIN DE LISTA
 * ---------------------------------------------------------------- */
#define END_ENEMY { ENEMY_NONE, 0, 0.0f,0.0f, 0.0f,0.0f, 0, 0.0f,0.0f, 0.0f,0.0f, 0,0 }

/* ----------------------------------------------------------------
 * TABLAS DE ENEMIGOS POR PANTALLA Y EPOCA
 * ---------------------------------------------------------------- */
static const Enemy enemies_pre[SCREEN_COUNT][MAX_ENEMIES] =
{
    /* P1: cima, sin enemigos */
    { END_ENEMY },

    /* P2: ladera, sin enemigos */
    { END_ENEMY },
    
    /* P3: pie colina, oso */
    {
        { ENEMY_BEAR, 1, 200.0f, 117.0f, 0.2f,0.0f,
          PAT_HORIZONTAL, 127.0f,270.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    
    /* P4: nivel medio, jabali */
    {
        { ENEMY_BOAR, 1, 100.0f, 128.0f, 0.8f,0.0f,
          PAT_HORIZONTAL, 40.0f,260.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    
    /* P5: hoguera, sin enemigos */
    { END_ENEMY },
    
    /* P6: zona baja izquierda, sin enemigos */
    { END_ENEMY },
    
    /* P7: zona baja centro, reptil */
    {
        { ENEMY_REPTILE, 1, 80.0f, 128.0f, 0.6f,0.0f,
          PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    
    /* P8: cruce del rio, peces */
    {
        { ENEMY_FISH, 1, 115.0f, 125.0f, 0.0f, 2.5f,
          PAT_VERTICAL, 0.0f,0.0f, 95.0f, 125.0f, 0,0 },
        { ENEMY_FISH, 1, 152.0f, 125.0f, 0.0f, 2.0f,
          PAT_VERTICAL, 0.0f,0.0f, 90.0f, 125.0f, 0,0 },
        { ENEMY_FISH, 1, 190.0f, 125.0f, 0.0f, 2.8f,
          PAT_VERTICAL, 0.0f,0.0f, 85.0f, 125.0f, 0,0 },
        END_ENEMY
    },
    /* P9: monolito, sin enemigos */
    { END_ENEMY },
};

static const Enemy enemies_med[SCREEN_COUNT][MAX_ENEMIES] =
{
    /* P1: megalitos, sin enemigos */
    { END_ENEMY },
    /* P2: ladera, sin enemigos */
    { END_ENEMY },
    /* P3: casa, sin enemigos */
    { END_ENEMY },
    /* P4: nivel medio, guardia */
    {
        { ENEMY_GUARD, 1, 100.0f,160.0f, 0.7f,0.0f,
          PAT_HORIZONTAL, 40.0f,260.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P5: capilla, sin enemigos */
    { END_ENEMY },
    /* P6: orilla foso, guardia */
    {
        { ENEMY_GUARD, 1, 80.0f,160.0f, 0.6f,0.0f,
          PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P7: foso centro, guardia */
    {
        { ENEMY_GUARD, 1, 120.0f,160.0f, 0.7f,0.0f,
          PAT_HORIZONTAL, 40.0f,260.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P8: puente levadizo, guardia */
    {
        { ENEMY_GUARD, 1, 60.0f,160.0f, 0.6f,0.0f,
          PAT_HORIZONTAL, 20.0f,130.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P9: torre, guardia con llave */
    {
        { ENEMY_GUARD, 1, 140.0f,160.0f, 0.7f,0.0f,
          PAT_HORIZONTAL, 80.0f,240.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
};

static const Enemy enemies_fut[SCREEN_COUNT][MAX_ENEMIES] =
{
    /* P1: cima, dron */
    {
        { ENEMY_DRONE, 1, 60.0f,100.0f, 1.0f,0.0f,
          PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P2: ladera, sin enemigos */
    { END_ENEMY },
    /* P3: pie colina, dron */
    {
        { ENEMY_DRONE, 1, 100.0f,120.0f, 1.0f,0.0f,
          PAT_HORIZONTAL, 40.0f,260.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P4: nivel medio, dron */
    {
        { ENEMY_DRONE, 1, 80.0f,110.0f, 1.2f,0.0f,
          PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P5: terminal, sin enemigos */
    { END_ENEMY },
    /* P6: escombros fragmento 3, robot */
    {
        { ENEMY_ROBOT, 1, 100.0f,160.0f, 0.5f,0.0f,
          PAT_HORIZONTAL, 40.0f,240.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P7: cauce seco, dron */
    {
        { ENEMY_DRONE, 1, 80.0f,110.0f, 1.0f,0.0f,
          PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P8: zona radiacion, robot */
    {
        { ENEMY_ROBOT, 1, 60.0f,160.0f, 0.5f,0.0f,
          PAT_HORIZONTAL, 20.0f,130.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    /* P9: Mecha, dos drones */
    {
        { ENEMY_DRONE, 1, 60.0f,100.0f, 1.2f,0.0f,
          PAT_HORIZONTAL, 20.0f,140.0f, 0.0f,0.0f, 0,0 },
        { ENEMY_DRONE, 1, 200.0f,120.0f, 1.0f,0.0f,
          PAT_HORIZONTAL, 160.0f,290.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
};

/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void enemies_init(void)
{
    int i;

    g_enemies.count = 0;
    for (i = 0; i < MAX_ENEMIES; i++)
        g_enemies.enemies[i].active = 0;
}

/* ----------------------------------------------------------------
 * LOAD
 * Carga los enemigos de la pantalla actual
 * ---------------------------------------------------------------- */

/* Ultima direccion del oso antes de detenerse (para dibujo correcto) */
static float s_bear_last_dir = -1.0f;  /* -1=izquierda, 1=derecha */

void enemies_load(int epoch, int screen)
{
    int i;
    const Enemy *src;

    switch (epoch)
    {
        case EPOCH_PREHISTORY: src = enemies_pre[screen]; break;
        case EPOCH_MEDIEVAL:   src = enemies_med[screen]; break;
        case EPOCH_FUTURE:     src = enemies_fut[screen]; break;
        default:               src = enemies_pre[screen]; break;
    }

    g_enemies.count = 0;
    for (i = 0; i < MAX_ENEMIES; i++)
    {
        if (src[i].type == ENEMY_NONE) break;
        g_enemies.enemies[i] = src[i];
        /* Si el puzzle del oso ya esta resuelto, desactivarlo al cargar */
        if (src[i].type == ENEMY_BEAR && puzzle_is_solved(PUZZLE_BEAR))
            g_enemies.enemies[i].active = 0;
        g_enemies.count++;
    }

    /* Si el puzzle del oso esta resuelto, eliminar la taza con miel del suelo */
    if (puzzle_is_solved(PUZZLE_BEAR))
        inv_remove_instance(ITEM_CUP_HONEY, epoch, screen);
}

/* ----------------------------------------------------------------
 * UPDATE
 * Mueve los enemigos segun su patron
 * ---------------------------------------------------------------- */
void enemies_update(void)
{
    int i;
    Enemy *e;

    for (i = 0; i < g_enemies.count; i++)
    {
        e = &g_enemies.enemies[i];
        if (!e->active) continue;

        /* Oso con puzzle resuelto: camina hacia la taza y se detiene */
        if (e->type == ENEMY_BEAR && puzzle_is_solved(PUZZLE_BEAR))
        {
            if (e->vel_x < -0.01f || e->vel_x > 0.01f)
            {
                s_bear_last_dir = (e->vel_x < 0.0f) ? -1.0f : 1.0f;
                e->x += e->vel_x;
                if ((e->vel_x < 0.0f && e->x <= e->min_x) ||
                    (e->vel_x > 0.0f && e->x >= e->min_x))
                {
                    e->x          = e->min_x;
                    e->vel_x      = 0.0f;
                    e->anim_frame = 0;
                    e->anim_timer = 0;
                    sfx_free(1);
                }
                else if (++e->anim_timer >= 15)
                {
                    e->anim_timer = 0;
                    e->anim_frame = (e->anim_frame + 1) % 3;
                    if (e->anim_frame == 0)
                        sfx_play(SFX_BEAR_STEP, 38, MIDDLE);
                }
            }
            continue;
        }

        switch (e->pattern)
        {
            case PAT_HORIZONTAL:
                e->x += e->vel_x;
                if (e->x <= e->min_x)
                {
                    e->x     = e->min_x;
                    e->vel_x = -e->vel_x;
                }
                if (e->x >= e->max_x)
                {
                    e->x     = e->max_x;
                    e->vel_x = -e->vel_x;
                }
                break;

            case PAT_VERTICAL:
                e->y += e->vel_y;
                if (e->y <= e->min_y)
                {
                    e->y     = e->min_y;
                    e->vel_y = -e->vel_y;
                }
                if (e->y >= e->max_y)
                {
                    e->y     = e->max_y;
                    e->vel_y = -e->vel_y;
                }
                break;

            case PAT_FIXED:
            default:
                break;
        }

        if (++e->anim_timer >= 15)
        {
            e->anim_timer = 0;
            if (e->type == ENEMY_BEAR)
            {
                e->anim_frame = (e->anim_frame + 1) % 3;
                /* Pisada en frames 0 y 2 (apoyos de pata delantera y trasera) */
                if (e->anim_frame == 0/* || e->anim_frame == 2*/)
                    sfx_play(SFX_BEAR_STEP, 38, MIDDLE);
            }
            else
                e->anim_frame = (e->anim_frame + 1) % 3;
        }
    }
}

/* ----------------------------------------------------------------
 * CHECK COLLISION
 * Devuelve 1 si Eric toca algun enemigo activo
 * ---------------------------------------------------------------- */
int enemies_check_collision(void)
{
    int i;
    int px, py, ex, ey, ew, eh;
    Enemy *e;

    /* TEMP: desactivado para pruebas */
    return 0; 

    px = (int)g_game.player.x;
    py = (int)g_game.player.y;

    for (i = 0; i < g_enemies.count; i++)
    {
        e = &g_enemies.enemies[i];
        if (!e->active) continue;

        ex = (int)e->x;
        ey = (int)e->y;
        ew = ENEMY_W(e->type);
        eh = ENEMY_H(e->type);

        if (px + PLAYER_WIDTH - 4 <= ex)  continue;
        if (px + 4 >= ex + ew)            continue;
        if (py + PLAYER_HEIGHT - 4 <= ey) continue;
        if (py + 4 >= ey + eh)            continue;

        return 1;
    }
    return 0;
}

/* ----------------------------------------------------------------
 * DRAW
 * Dibuja los enemigos como placeholders de rectangulos rojos
 * ---------------------------------------------------------------- */
void enemies_draw(void)
{
    int i;
    int x, y, w, h;
    int sx, sy, dx, dy;
    unsigned char c;
    Enemy *e;

    for (i = 0; i < g_enemies.count; i++)
    {
        e = &g_enemies.enemies[i];
        if (!e->active) continue;

        x = (int)e->x;
        y = (int)e->y;
        w = ENEMY_W(e->type);
        h = ENEMY_H(e->type);

        /* Oso: dibujar sprite real con espejado segun direccion */
        if (e->type == ENEMY_BEAR && g_bear_sprite.data != NULL)
        {
            float draw_dir = (e->vel_x != 0.0f) ? e->vel_x : s_bear_last_dir;
            screen_inject_bear_palette();

            if (draw_dir >= 0.0f)
            {
                /* Mirando a la derecha */
                bmp_draw_tile(&g_bear_sprite, e->anim_frame, 0,
                              48, 32, x, y);
            }
            else
            {
                /* Mirando a la izquierda: espejado horizontal */
                for (sy = 0; sy < 32; sy++)
                {
                    dy = y + sy;
                    if (dy < 0 || dy >= SCREEN_H) continue;
                    for (sx = 0; sx < 48; sx++)
                    {
                        dx = x + (47 - sx);
                        if (dx < 0 || dx >= SCREEN_W) continue;
                        c = g_bear_sprite.data[sy * g_bear_sprite.width +
                            e->anim_frame * 48 + sx];
                        if (c == 0) continue;
                        back_buffer[dy * SCREEN_W + dx] = c;
                    }
                }
            }
            continue;
        }

        /* Placeholder para el resto de enemigos */
        draw_line(x,     y,     x + w, y,     4);
        draw_line(x + w, y,     x + w, y + h, 4);
        draw_line(x + w, y + h, x,     y + h, 4);
        draw_line(x,     y + h, x,     y,     4);
    }
}
