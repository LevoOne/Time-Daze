/* ---------------------------------------------------------
 * enemies.c - Sistema de enemigos de Time Daze
 * C89: todas las variables declaradas al inicio del bloque.
 * --------------------------------------------------------- */

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
        { ENEMY_BOAR, 1, 100.0f, 84.0f, 0.8f,0.0f,
          PAT_HORIZONTAL, 4.0f,220.0f, 0.0f,0.0f, 0,0 },
        END_ENEMY
    },
    
    /* P5: hoguera, sin enemigos */
    { END_ENEMY },
    
    /* P6: zona baja izquierda, sin enemigos */
    { END_ENEMY },
    
    /* P7: zona baja centro, reptil */
    {
        { ENEMY_REPTILE, 1, 80.0f, 124.0f, 0.15f,0.0f, PAT_HORIZONTAL, 20.0f,200.0f, 0.0f,0.0f, 0,0 },

        END_ENEMY
    },
    
    /* P8: cruce del rio, peces */

    /* Mi propuesta de patrón de movimiento
    {
        { ENEMY_FISH, 1, 110.0f, 125.0f, 0.0f, -6.0f, PAT_VERTICAL, -6.0f, 0.15f, 14.0f, 125.0f, 0,0 },

        { ENEMY_FISH, 1, 145.0f, 125.0f, 0.0f, -3.5f, PAT_VERTICAL, -3.5f, 0.06f, 84.0f, 125.0f, 0,0 },

        { ENEMY_FISH, 1, 186.0f, 125.0f, 0.0f, -5.3f, PAT_VERTICAL, -5.3f, 0.10f, 24.0f, 125.0f, 0,0 },

        END_ENEMY
    },
    */

    /* Alternativa, funciona mejor con el Bounding Box cambiado */
    {
        //{ ENEMY_FISH, 1, 110.0f, 125.0f, 0.0f, -4.0f, PAT_VERTICAL, -4.0f, 0.15f, 55.0f, 125.0f, 0,0 },
        { ENEMY_FISH, 1, 110.0f, 125.0f, 0.0f, -3.0f, PAT_VERTICAL, -3.0f, 0.15f, 55.0f, 125.0f, 0,0 }, 
        { ENEMY_FISH, 1, 145.0f, 125.0f, 0.0f, -3.5f, PAT_VERTICAL, -3.5f, 0.06f, 84.0f, 125.0f, 0,0 },
        { ENEMY_FISH, 1, 186.0f, 125.0f, 0.0f, -3.8f, PAT_VERTICAL, -3.8f, 0.10f, 65.0f, 125.0f, 0,0 },
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
        { ENEMY_DRONE, 1, 290.0f,120.0f, 1.5f,0.8f, PAT_ZIGZAG, 10.0f,280.0f,80.0f,120.0f, 0,0 },
        END_ENEMY
    },

    /* P2: ladera, sin enemigos */
    {
        END_ENEMY
    },

    /* P3: pie colina, dron */
    {
        { ENEMY_DRONE, 1, 290.0f,120.0f, 1.8f,3.5f, PAT_ZIGZAG, 20.0f,280.0f, 30.0f,120.0f, 0,0 },
        END_ENEMY
    },
    /* P4: nivel medio, dron */
    {
        { ENEMY_DRONE, 1, 80.0f,110.0f, 1.2f,0.0f, PAT_HORIZONTAL, 20.0f,280.0f, 0.0f,0.0f, 0,0 },
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

/* Ultima direccion del jabali (para dibujo correcto, mismo criterio que el oso) */
static float s_boar_last_dir = -1.0f;  /* -1=izquierda, 1=derecha */

/* Última dirección del reptil (para dibujo correct, mismo criterio que el oso) */
static float s_reptile_last_dir = -1.0f; /* -1=izquierda, 1=derecha */

/* Temporizador de salpicadura por pez (uno por slot de enemigo).
 * Se activa al tocar el agua (ver PAT_VERTICAL en enemies_update)
 * y cuenta hacia atras en cada frame hasta llegar a 0. */
#define FISH_SPLASH_DURATION 8

#define FISH_WAIT_MIN  10   /* frames minimos de espera en el agua */
#define FISH_WAIT_MAX  40   /* frames maximos de espera en el agua */

static int s_fish_splash_timer[MAX_ENEMIES];

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

        /* Actualizo el timer de las salpicaduras */
         if (s_fish_splash_timer[i] > 0)
            s_fish_splash_timer[i]--;

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
                /* Fisica de salto con gravedad real:
                 *   min_x = velocidad de lanzamiento inicial (negativa,
                 *           hacia arriba)
                 *   max_x = gravedad (aceleracion que se suma cada frame)
                 * min_y = altura maxima del salto (tope de seguridad)
                 * max_y = nivel del agua (punto de relanzamiento)       */
                e->vel_y += e->max_x;
                e->y     += e->vel_y;

                if (e->y >= e->max_y)
                {
                    e->y     = e->max_y;
                    e->vel_y = e->min_x;
                    if (e->type == ENEMY_FISH) {
                        s_fish_splash_timer[i] = FISH_SPLASH_DURATION;
                        sfx_play(SFX_BEAR_STEP, 38, MIDDLE);
                    }
                }
                if (e->y <= e->min_y)
                {
                    e->y = e->min_y;
                }
                break;

            case PAT_ZIGZAG:
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
            else if (e->type == ENEMY_BOAR)
            {
                /* BOAR.BMP tiene 4 frames reales de animación */
                e->anim_frame = (e->anim_frame + 1) % 4;
                if (e->anim_frame == 0 || e->anim_frame == 3)
                    sfx_play(SFX_BEAR_STEP, 39, MIDDLE);
            }
            else if (e->type == ENEMY_REPTILE)
            {
                /* REPTILE.BMP también tiene 4 frames */
                e->anim_frame = (e->anim_frame + 1) % 3;
                if(e->anim_frame == 1)
                    sfx_play(SFX_BEAR_STEP, 38, MIDDLE);
            }
            else if (e->type == ENEMY_DRONE)
            {
                /* DRONE.BMP solo 3 frames */
                e->anim_frame = (e->anim_frame + 1) % 3;
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

    px = (int)g_game.player.x;
    py = (int)g_game.player.y;

    for (i = 0; i < g_enemies.count; i++)
    {
        e = &g_enemies.enemies[i];
        if (!e->active) continue;

        ex = (int)e->x + ENEMY_XOFF(e->type);
        ey = (int)e->y + ENEMY_YOFF(e->type);
        ew = ENEMY_W(e->type);
        eh = ENEMY_H(e->type);

        /* Check de Bounding Box para jabali, oso y lagarto */
        if(e->type != ENEMY_FISH)
        {
            
            if (px + PLAYER_WIDTH - 4 <= ex)  continue;
            if (px +4 >= ex + ew)            continue; 
            if (py + PLAYER_HEIGHT - 4 <= ey) continue;
            if (py + 4 >= ey + eh)            continue;
        }
        else
        {
            /* Check de Bounding Box para peces */
            if (px + PLAYER_WIDTH - 14 <= ex)  continue;
            if (px + 3 >= ex + ew)            continue;
            if (py - 0 <= ey) continue;
            if (py + 0 >= ey + eh)            continue;
        }

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

        /* Jabali: dibujar sprite real con espejado segun direccion */
        if (e->type == ENEMY_BOAR && g_boar_sprite.data != NULL)
        {
            float draw_dir = (e->vel_x != 0.0f) ? e->vel_x : s_boar_last_dir;
            screen_inject_boar_palette();

            if (draw_dir >= 0.0f)
            {
                s_boar_last_dir = 1.0f;
                /* Mirando a la derecha */
                bmp_draw_tile(&g_boar_sprite, e->anim_frame, 0,
                              64, 64, x, y);
            }
            else
            {
                s_boar_last_dir = -1.0f;
                /* Mirando a la izquierda: espejado horizontal */
                for (sy = 0; sy < 64; sy++)
                {
                    dy = y + sy;
                    if (dy < 0 || dy >= SCREEN_H) continue;
                    for (sx = 0; sx < 64; sx++)
                    {
                        dx = x + (63 - sx);
                        if (dx < 0 || dx >= SCREEN_W) continue;
                        c = g_boar_sprite.data[sy * g_boar_sprite.width +
                            e->anim_frame * 64 + sx];
                        if (c == 0) continue;
                        back_buffer[dy * SCREEN_W + dx] = c;
                    }
                }
            }
            continue;
        }

        /* Reptil: dibujar sprite real con espejado segun direccion */
        if (e->type == ENEMY_REPTILE && g_reptile_sprite.data != NULL)
        {
            float draw_dir = (e->vel_x != 0.0f) ? e->vel_x : s_reptile_last_dir;
            screen_inject_reptile_palette();

            if (draw_dir >= 0.0f)
            {
                s_reptile_last_dir = 1.0f;
                /* Mirando a la derecha */
                bmp_draw_tile(&g_reptile_sprite, e->anim_frame, 0,
                              64, 24, x, y);
            }
            else
            {
                s_reptile_last_dir = -1.0f;
                /* Mirando a la izquierda: espejado horizontal */
                for (sy = 0; sy < 24; sy++)
                {
                    dy = y + sy;
                    if (dy < 0 || dy >= SCREEN_H) continue;
                    for (sx = 0; sx < 64; sx++)
                    {
                        dx = x + (63 - sx);
                        if (dx < 0 || dx >= SCREEN_W) continue;
                        c = g_reptile_sprite.data[sy * g_reptile_sprite.width +
                            e->anim_frame * 64 + sx];
                        if (c == 0) continue;
                        back_buffer[dy * SCREEN_W + dx] = c;
                    }
                }
            }
            continue;
        }

        /* Peces: dibujar sprite real, espejado en vertical segun suba o baje */
        if (e->type == ENEMY_FISH && g_fish_sprite.data != NULL)
        {
            if (s_fish_splash_timer[i] > 0 && g_splash_sprite.data != NULL)
            {
                /* Salpicadura: usa los mismos indices de color que el agua
                 * de fondo (242-255), no requiere inyectar ninguna paleta */
                bmp_draw_tile(&g_splash_sprite, 0, 0, 24, 12,
                              (int)e->x, (int)e->max_y + 13);
                continue;
            }
            
            screen_inject_fish_palette();

            if (e->vel_y <= 0.0f)
            {
                /* Subiendo: dibujo normal */
                bmp_draw_tile(&g_fish_sprite, e->anim_frame, 0, 30, 25, x, y);
            }
            else
            {
                /* Bajando: espejado vertical (voltea filas, no columnas) */
                for (sy = 0; sy < 25; sy++)
                {
                    dy = y + (24 - sy);
                    if (dy < 0 || dy >= SCREEN_H) continue;
                    for (sx = 0; sx < 30; sx++)
                    {
                        dx = x + sx;
                        if (dx < 0 || dx >= SCREEN_W) continue;
                        c = g_fish_sprite.data[sy * g_fish_sprite.width +
                            e->anim_frame * 30 + sx];
                        if (c == 0) continue;
                        back_buffer[dy * SCREEN_W + dx] = c;
                    }
                }
            }
            continue;
        }

        /* Dron: como es una esfera girando sobre si misma, no hay que espejar por dirección. Sin espejado por direccion ya que el
            giro del contenido interior ya transmite el movimiento, a diferencia de un animal que necesita mirar hacia donde camina. */
        if (e->type == ENEMY_DRONE && g_drone_sprite.data != NULL)
        {
            screen_inject_drone_palette();
            bmp_draw_tile(&g_drone_sprite, e->anim_frame, 0, 32, 32, x, y);
            continue;
        }

        /* Placeholder para el resto de enemigos */
        draw_line(x,     y,     x + w, y,     4);
        draw_line(x + w, y,     x + w, y + h, 4);
        draw_line(x + w, y + h, x,     y + h, 4);
        draw_line(x,     y + h, x,     y,     4);
    }
}
