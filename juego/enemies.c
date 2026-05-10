/*
 * enemies.c - Sistema de enemigos de Time Daze
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "engine.h"
#include "game.h"
#include "enemies.h"
#include "player.h"
#include "puzzles.h"





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
        { ENEMY_BEAR, 1, 200.0f, 116.0f, 0.5f, 0.0f,
        PAT_HORIZONTAL, 180.0f, 295.0f, 0.0f, 0.0f, 0, 0 },
        END_ENEMY
    },
    
    /* P4: nivel medio, jabali */
    {
        { ENEMY_BOAR, 1, 100.0f,116.0f, 0.8f,0.0f,
          PAT_HORIZONTAL, 40.0f,260.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    
    /* P5: hoguera, sin enemigos */
    { END_ENEMY },
    
    /* P6: zona baja, sin enemigos */
    { END_ENEMY },
    
    /* P7: zona baja centro, reptil */
    {
        { ENEMY_REPTILE, 1, 80.0f, 116.0f, 0.6f,0.0f,
          PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    
    /* P8: cruce del rio, peces */
    {
        { ENEMY_FISH, 1, 100.0f,170.0f, 0.0f,2.5f,
          PAT_VERTICAL, 0.0f,0.0f, 130.0f,175.0f, 0,0 },
        { ENEMY_FISH, 1, 150.0f,175.0f, 0.0f,2.0f,
          PAT_VERTICAL, 0.0f,0.0f, 135.0f,175.0f, 0,0 },
        { ENEMY_FISH, 1, 200.0f,172.0f, 0.0f,2.8f,
          PAT_VERTICAL, 0.0f,0.0f, 128.0f,175.0f, 0,0 },
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
        g_enemies.count++;
    }
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

        if (++e->anim_timer >= 8)
        {
            e->anim_timer = 0;
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
    int px, py, ex, ey;
    Enemy *e;

    /* Temporal solo para Debug */
    return 0;
    
    px = (int)g_game.player.x;
    py = (int)g_game.player.y;

    for (i = 0; i < g_enemies.count; i++)
    {
        e = &g_enemies.enemies[i];
        if (!e->active) continue;

        ex = (int)e->x;
        ey = (int)e->y;

        if (px + PLAYER_WIDTH - 4 <= ex)     continue;
        if (px + 4 >= ex + PLAYER_WIDTH)     continue;
        if (py + PLAYER_HEIGHT - 4 <= ey)     continue;
        if (py + 4 >= ey + PLAYER_HEIGHT)     continue;

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
    int x, y;
    Enemy *e;

    for (i = 0; i < g_enemies.count; i++)
    {
        e = &g_enemies.enemies[i];
        if (!e->active) continue;

        x = (int)e->x;
        y = (int)e->y;

        draw_line(x,            y,            x + PLAYER_WIDTH, y,            4);
        draw_line(x + PLAYER_WIDTH, y,            x + PLAYER_WIDTH, y + PLAYER_HEIGHT, 4);
        draw_line(x + PLAYER_WIDTH, y + PLAYER_HEIGHT, x,            y + PLAYER_HEIGHT, 4);
        draw_line(x,            y + PLAYER_HEIGHT, x,            y,            4);
    }
}
