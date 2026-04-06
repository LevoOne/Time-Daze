/*
 * hud.c - Sistema de HUD de Tempus Fugit
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include <string.h>
#include "engine.h"
#include "game.h"
#include "hud.h"
#include "puzzles.h"





/* ----------------------------------------------------------------
 * VARIABLE GLOBAL
 * ---------------------------------------------------------------- */
HudState g_hud;

/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void hud_init(void)
{
    g_hud.mode           = HUD_NORMAL;
    g_hud.mecha_lives    = MECHA_LIVES;
    g_hud.cristal_ticks  = 0;
    g_hud.cristal_active = 0;
}

/* ----------------------------------------------------------------
 * SET MODE
 * ---------------------------------------------------------------- */
void hud_set_mode(int mode)
{
    g_hud.mode = mode;
}

/* ----------------------------------------------------------------
 * MECHA HIT
 * ---------------------------------------------------------------- */
void hud_mecha_hit(void)
{
    if (g_hud.mecha_lives > 0)
        g_hud.mecha_lives--;
}

/* ----------------------------------------------------------------
 * MECHA DESTROYED
 * ---------------------------------------------------------------- */
int hud_mecha_destroyed(void)
{
    return g_hud.mecha_lives <= 0;
}

/* ----------------------------------------------------------------
 * CRISTAL ACTIVATE
 * ---------------------------------------------------------------- */
void hud_cristal_activate(void)
{
    g_hud.cristal_ticks  = CRISTAL_CARGA_TICKS;
    g_hud.cristal_active = 1;
}

/* ----------------------------------------------------------------
 * CRISTAL UPDATE
 * Devuelve 1 si sigue activo, 0 si se ha agotado
 * ---------------------------------------------------------------- */
int hud_cristal_update(void)
{
    if (!g_hud.cristal_active) return 0;

    g_hud.cristal_ticks--;
    if (g_hud.cristal_ticks <= 0)
    {
        g_hud.cristal_active = 0;
        return 0;
    }
    return 1;
}

/* ----------------------------------------------------------------
 * DRAW NORMAL
 * ---------------------------------------------------------------- */
static void hud_draw_normal(void)
{
    int i, x;
    unsigned char color;

    /* Fondo negro de la franja del HUD */
    for (i = HUD_Y; i < HUD_Y + HUD_HEIGHT; i++)
        memset(&back_buffer[i * SCREEN_W], 0, SCREEN_W);

    /* Vidas: rectangulos blancos */
    for (i = 0; i < g_game.player.lives; i++)
    {
        x = 4 + i * 12;
        draw_line(x,     HUD_Y + 3,  x + 8, HUD_Y + 3,  15);
        draw_line(x + 8, HUD_Y + 3,  x + 8, HUD_Y + 11, 15);
        draw_line(x + 8, HUD_Y + 11, x,     HUD_Y + 11, 15);
        draw_line(x,     HUD_Y + 11, x,     HUD_Y + 3,  15);
    }

    /* Objeto actual: rectangulo amarillo si lleva algo */
    if (g_game.inv.carried != ITEM_NONE)
    {
        x = 60;
        draw_line(x,     HUD_Y + 3,  x + 8, HUD_Y + 3,  14);
        draw_line(x + 8, HUD_Y + 3,  x + 8, HUD_Y + 11, 14);
        draw_line(x + 8, HUD_Y + 11, x,     HUD_Y + 11, 14);
        draw_line(x,     HUD_Y + 11, x,     HUD_Y + 3,  14);
    }

    /* Fragmentos: grisaceo si no recogido, color si recogido */
    for (i = 0; i < FRAGMENT_COUNT; i++)
    {
        x = 290 - (FRAGMENT_COUNT - 1 - i) * 14;

        if (fragment_is_collected(i))
        {
            /* Color distintivo por fragmento: 2=verde, 3=cyan, 4=rojo */
            color = (i == 0) ? 2 : (i == 1) ? 3 : 4;
        }
        else
        {
            color = 8;   /* grisaceo */
        }

        draw_line(x,     HUD_Y + 3,  x + 8, HUD_Y + 3,  color);
        draw_line(x + 8, HUD_Y + 3,  x + 8, HUD_Y + 11, color);
        draw_line(x + 8, HUD_Y + 11, x,     HUD_Y + 11, color);
        draw_line(x,     HUD_Y + 11, x,     HUD_Y + 3,  color);
    }
}

/* ----------------------------------------------------------------
 * DRAW MECHA
 * ---------------------------------------------------------------- */
static void hud_draw_mecha(void)
{
    int i, x;
    int segundos;
    unsigned char color;

    /* Fondo azul oscuro para el HUD del Mecha */
    for (i = HUD_Y; i < HUD_Y + HUD_HEIGHT; i++)
        memset(&back_buffer[i * SCREEN_W], 1, SCREEN_W);

    /* Estado del Mecha: tres iconos */
    for (i = 0; i < MECHA_LIVES; i++)
    {
        x     = 4 + i * 12;
        color = (g_hud.mecha_lives > i) ? 14 : 8;

        draw_line(x,     HUD_Y + 3,  x + 8, HUD_Y + 3,  color);
        draw_line(x + 8, HUD_Y + 3,  x + 8, HUD_Y + 11, color);
        draw_line(x + 8, HUD_Y + 11, x,     HUD_Y + 11, color);
        draw_line(x,     HUD_Y + 11, x,     HUD_Y + 3,  color);
    }

    /* Contador del cristal */
    if (g_hud.cristal_active)
    {
        segundos = g_hud.cristal_ticks / 70;

        if      (segundos <= 5)  color = 4;    /* rojo    */
        else if (segundos <= 15) color = 14;   /* amarillo */
        else                     color = 15;   /* blanco  */

        /* Placeholder hasta implementar draw_number */
        draw_line(140, HUD_Y + 3,  160, HUD_Y + 3,  color);
        draw_line(140, HUD_Y + 7,  160, HUD_Y + 7,  color);
        draw_line(140, HUD_Y + 11, 160, HUD_Y + 11, color);
    }
}

/* ----------------------------------------------------------------
 * DRAW
 * ---------------------------------------------------------------- */
void hud_draw(void)
{
    if (g_hud.mode == HUD_MECHA)
        hud_draw_mecha();
    else
        hud_draw_normal();
}
