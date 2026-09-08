/*
 * hud.c - Sistema de HUD de Tempus Fugit
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include <string.h>
#include "engine.h"
#include "game.h"
#include "hud.h"
#include "puzzles.h"
#include "screen.h"

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
    g_hud.nearby_item    = ITEM_NONE;
}

/* ----------------------------------------------------------------
 * SET MODE
 * ---------------------------------------------------------------- */
void hud_set_mode(int mode)
{
    g_hud.mode = mode;
}

void hud_set_nearby_item(int item_id)
{
    g_hud.nearby_item = item_id;
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
    const char* item_name;
    int icon_w = 0;
    int iEpochPal = 0;
    unsigned char color_text;

    /* Determinamos color fuente HUD según época */
    switch (g_game.screen.current_epoch)
    {
        case EPOCH_PREHISTORY: color_text = 37; break;  
        case EPOCH_MEDIEVAL:   color_text = 96; break;  
        case EPOCH_FUTURE:     color_text = 37; break;  
        default:               color_text = 37;
    }


    /* Fondo negro de la franja del HUD */
    switch(g_game.screen.current_epoch)
    {
        case EPOCH_PREHISTORY: iEpochPal = 129; break;
        case EPOCH_MEDIEVAL: iEpochPal = 129; break;
        default: break;
    }

    for (i = HUD_Y; i < HUD_Y + HUD_HEIGHT; i++)
    {
       memset(&back_buffer[i * SCREEN_W], iEpochPal, SCREEN_W);
    }  

    /* Vidas: cabeza de Eric */
    for (i = 0; i < g_game.player.lives; i++)
    {
        x = 4 + i * 20;
        if (g_eric_head.data != NULL)
            draw_bitmap_buf_t(&g_eric_head, x, HUD_Y + 4);
    }

    /* Objeto cercano en el suelo: tiene prioridad temporal sobre lo que
     * Eric lleve encima, para avisar al jugador de que hay algo ahi
     * aunque tenga las manos ocupadas. En cuanto se aleje, vuelve a
     * verse lo que lleva con normalidad. */
    if (g_hud.nearby_item != ITEM_NONE)
    {
        /* Objeto cercano en el suelo: mostrar icono + nombre en color tenue */
        x = 60;
        icon_w = 0;

        if (g_hud.nearby_item == ITEM_STICK && g_stick_sprite.data != NULL)
        {
            icon_w = g_stick_sprite.width;
            draw_bitmap_buf_t(&g_stick_sprite, x+10, HUD_Y + 6);
        }
        else if (g_hud.nearby_item == ITEM_DINO_EGG && g_egg_icon.data != NULL)
        {
            icon_w = g_egg_icon.width;
            draw_bitmap_buf_t(&g_egg_icon, x + 10, HUD_Y + 4);
        }
        else if (g_hud.nearby_item == ITEM_CUP && g_cup_icon.data != NULL)
        {
            icon_w = g_cup_icon.width;
            draw_bitmap_buf_t(&g_cup_icon, x+10, HUD_Y + 4);
        }
        else if (g_hud.nearby_item == ITEM_CUP_HONEY && g_cuphoney_icon.data != NULL)
        {
            icon_w = g_cuphoney_icon.width;
            draw_bitmap_buf_t(&g_cuphoney_icon, x, HUD_Y + 4);
        }
        else if (g_hud.nearby_item == ITEM_LOG  && g_log_icon.data != NULL)
        {
            icon_w = g_log_icon.width;
            draw_bitmap_buf_t(&g_log_icon, x+10, HUD_Y + 4);
        }

        switch (g_hud.nearby_item)
        {
            case ITEM_STICK:        item_name = "palo";                 break;
            case ITEM_CUP:          item_name = "recipiente";           break;
            case ITEM_CUP_HONEY:    item_name = "recipiente con miel";  break;
            case ITEM_LOG:          item_name = "palanca";              break;
            case ITEM_DINO_EGG:     item_name = "huevo";                break;
            case ITEM_CRANK:        item_name = "MANIVELA";             break;
            case ITEM_KEY:          item_name = "LLAVE";                break;
            case ITEM_TORCH:        item_name = "ANTORCHA";             break;
            case ITEM_TORCH_LIT:    item_name = "ANTORCH.LIT";          break;
            case ITEM_LEVITATOR:    item_name = "LEVITADOR";            break;
            case ITEM_VOLCANIC_MIN: item_name = "MINERAL";              break;
            case ITEM_SEALANT:      item_name = "SELLANTE";             break;
            case ITEM_QUARTZ:       item_name = "CUARZO";               break;
            case ITEM_QUARTZ_CHARGED: item_name = "CUARZO+";            break;
            default:                item_name = "";                     break;
        }
        draw_string(item_name, x + icon_w + 14, HUD_Y + 10, color_text);
    }
    else if (g_game.inv.carried != ITEM_NONE)
    {
        x = 60;
        if (g_game.inv.carried == ITEM_STICK && g_stick_sprite.data != NULL)
        {
            icon_w = g_stick_sprite.width;
            draw_bitmap_buf_t(&g_stick_sprite, x+10, HUD_Y+6);
        }
        else if (g_game.inv.carried == ITEM_DINO_EGG && g_egg_icon.data != NULL)
        {
            icon_w = g_egg_icon.width;
            draw_bitmap_buf_t(&g_egg_icon, x+10, HUD_Y + 4);
        }
        else if (g_game.inv.carried == ITEM_CUP && g_cup_icon.data != NULL)
        {
            icon_w = g_cup_icon.width;
            draw_bitmap_buf_t(&g_cup_icon, x+10, HUD_Y + 4);
        }
        else if (g_game.inv.carried == ITEM_CUP_HONEY && g_cuphoney_icon.data != NULL)
        {
            icon_w = g_cuphoney_icon.width;
            draw_bitmap_buf_t(&g_cuphoney_icon, x+10, HUD_Y + 4);
        }
        else if (g_game.inv.carried == ITEM_LOG  && g_log_sprite.data != NULL)
        {
            icon_w = g_log_icon.width;
            draw_bitmap_buf_t(&g_log_icon, x+10, HUD_Y + 4);
        }
        else
        {
            draw_line(x,     HUD_Y + 3,  x + 8, HUD_Y + 3,  14);
            draw_line(x + 8, HUD_Y + 3,  x + 8, HUD_Y + 11, 14);
            draw_line(x + 8, HUD_Y + 11, x,     HUD_Y + 11, 14);
            draw_line(x,     HUD_Y + 11, x,     HUD_Y + 3,  14);
        }

        /* Imprimimos una cadena con el nombre del objeto del inventario */
        switch(inv_get_carried())
        {
            case ITEM_STICK:        item_name = "palo";                 break;
            case ITEM_CUP:          item_name = "recipiente";           break;
            case ITEM_CUP_HONEY:    item_name = "recipiente con miel";  break;
            case ITEM_LOG:          item_name = "palanca";              break;
            case ITEM_DINO_EGG:     item_name = "huevo";                break;
            case ITEM_CRANK:        item_name = "MANIVELA";             break;
            case ITEM_KEY:          item_name = "LLAVE";                break;
            case ITEM_TORCH:        item_name = "ANTORCHA";             break;
            case ITEM_TORCH_LIT:    item_name = "ANTORCH.LIT";          break;
            case ITEM_LEVITATOR:    item_name = "LEVITADOR";            break;
            case ITEM_VOLCANIC_MIN: item_name = "MINERAL";              break;
            case ITEM_SEALANT:      item_name = "SELLANTE";             break;
            case ITEM_QUARTZ:       item_name = "CUARZO";               break;
            case ITEM_QUARTZ_CHARGED: item_name = "CUARZO+";            break;
            default:                item_name = "";                     break;
        }

        draw_string(item_name, x + icon_w + 14, HUD_Y + 10, color_text);
    }

    /* Fragmentos: sprite apagado (gris, paleta de Eric) si no recogido,
     * sprite a color (dorado, paleta de la taza) si recogido. Cada
     * fragmento tiene su propia silueta; ninguno necesita inyeccion
     * de paleta porque ambos estados reutilizan rangos permanentes
     * ya activos (Eric 130-185 / taza 186-199), igual que el tronco. */
    {
        BITMAP *dim_sprites[3]       = { &g_frag1_dim, &g_frag2_dim, &g_frag3_dim };
        BITMAP *collected_sprites[3] = { &g_frag1_collected, &g_frag2_collected, &g_frag3_collected };

        for (i = 0; i < FRAGMENT_COUNT; i++)
        {
            x = 296 - (FRAGMENT_COUNT - 1 - i) * 22;

            if (fragment_is_collected(i))
                draw_bitmap_buf_t(collected_sprites[i], x, HUD_Y + 5);
            else
                draw_bitmap_buf_t(dim_sprites[i], x, HUD_Y + 5);
        }
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
