/*
 * inventory.c - Sistema de inventario de Tempus Fugit
 *
 * Gestiona el objeto que lleva Eric y los objetos
 * depositados en el mapa.
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "engine.h"
#include "game.h"
#include "screen.h"
#include "inventory.h"





/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void inv_init(void)
{
    int i;

    g_game.inv.carried        = ITEM_NONE;
    g_game.inv.instance_count = 0;
    for (i = 0; i < MAX_ITEM_INSTANCES; i++)
    {
        g_game.inv.instances[i].item_id = ITEM_NONE;
        g_game.inv.instances[i].active  = 0;
    }
}

/* ----------------------------------------------------------------
 * INV PLACE
 * Coloca un objeto en el mapa sin que Eric lo lleve.
 * Usar en logic_init para inicializar los objetos del mundo.
 * Devuelve 1 si OK, 0 si no hay slots libres.
 * ---------------------------------------------------------------- */
int inv_place(int item_id, int epoch, int screen, int x, int y)
{
    int i;
    ItemInstance *inst;

    if (g_game.inv.instance_count >= MAX_ITEM_INSTANCES) return 0;

    inst = NULL;
    for (i = 0; i < MAX_ITEM_INSTANCES; i++)
    {
        if (!g_game.inv.instances[i].active)
        {
            inst = &g_game.inv.instances[i];
            break;
        }
    }
    if (inst == NULL) return 0;

    inst->item_id = item_id;
    inst->epoch   = epoch;
    inst->screen  = screen;
    inst->x       = x;
    inst->y       = y;
    inst->active  = 1;
    g_game.inv.instance_count++;
    return 1;
}

/* ----------------------------------------------------------------
 * INV REMOVE INSTANCE
 * Desactiva el ItemInstance de un objeto en el mapa.
 * Llamar justo antes de inv_pick para que el objeto desaparezca.
 * ---------------------------------------------------------------- */
void inv_remove_instance(int item_id, int epoch, int screen)
{
    int i;
    ItemInstance *inst;

    for (i = 0; i < MAX_ITEM_INSTANCES; i++)
    {
        inst = &g_game.inv.instances[i];
        if (!inst->active)          continue;
        if (inst->item_id != item_id) continue;
        if (inst->epoch   != epoch)   continue;
        if (inst->screen  != screen)  continue;

        inst->active = 0;
        if (g_game.inv.instance_count > 0)
            g_game.inv.instance_count--;
        return;
    }
}

/* ----------------------------------------------------------------
 * INV PICK
 * Eric recoge un objeto. Devuelve 1 si OK, 0 si ya llevaba algo
 * ---------------------------------------------------------------- */
int inv_pick(int item_id)
{
    if (g_game.inv.carried != ITEM_NONE) return 0;
    g_game.inv.carried = item_id;
    return 1;
}

/* ----------------------------------------------------------------
 * INV DROP
 * Eric suelta el objeto en una posicion del mapa.
 * Devuelve 1 si OK, 0 si no llevaba nada o no hay slots libres
 * ---------------------------------------------------------------- */
int inv_drop(int epoch, int screen, int x, int y)
{
    int i;
    ItemInstance *inst;

    if (g_game.inv.carried == ITEM_NONE)            return 0;
    if (g_game.inv.instance_count >= MAX_ITEM_INSTANCES) return 0;

    inst = NULL;
    for (i = 0; i < MAX_ITEM_INSTANCES; i++)
    {
        if (!g_game.inv.instances[i].active)
        {
            inst = &g_game.inv.instances[i];
            break;
        }
    }
    if (inst == NULL) return 0;

    inst->item_id = g_game.inv.carried;
    inst->epoch   = epoch;
    inst->screen  = screen;
    inst->x       = x;
    inst->y       = y;
    inst->active  = 1;
    g_game.inv.carried = ITEM_NONE;
    g_game.inv.instance_count++;
    return 1;
}

/* ----------------------------------------------------------------
 * INV TRANSFORM
 * Transforma el objeto que lleva Eric en otro
 * ---------------------------------------------------------------- */
void inv_transform(int new_item_id)
{
    g_game.inv.carried = new_item_id;
}

/* ----------------------------------------------------------------
 * INV GET CARRIED
 * ---------------------------------------------------------------- */
int inv_get_carried(void)
{
    return g_game.inv.carried;
}

/* ----------------------------------------------------------------
 * INV IS CARRYING
 * ---------------------------------------------------------------- */
int inv_is_carrying(int item_id)
{
    return g_game.inv.carried == item_id;
}

/* ----------------------------------------------------------------
 * INV GET AT
 * Busca un objeto depositado cerca de unas coordenadas.
 * Devuelve puntero al ItemInstance o NULL si no hay nada.
 * ---------------------------------------------------------------- */
ItemInstance *inv_get_at(int epoch, int screen, int x, int y)
{
    int i;
    int dx, dy;
    ItemInstance *inst;

    for (i = 0; i < MAX_ITEM_INSTANCES; i++)
    {
        inst = &g_game.inv.instances[i];
        if (!inst->active)          continue;
        if (inst->epoch  != epoch)  continue;
        if (inst->screen != screen) continue;

        dx = inst->x - x;
        dy = inst->y - y;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (dx > 8) continue;
        if (dy > 8) continue;

        return inst;
    }
    return NULL;
}

/* ----------------------------------------------------------------
 * INV DRAW
 * Dibuja los objetos depositados en la pantalla actual
 * como placeholders de rectangulos amarillos
 * ---------------------------------------------------------------- */
void inv_draw(void)
{
    int i;
    int x, y;
    ItemInstance *inst;
    int epoch;
    int screen;

    epoch  = g_game.screen.current_epoch;
    screen = g_game.screen.current_screen;

    for (i = 0; i < MAX_ITEM_INSTANCES; i++)
    {
        inst = &g_game.inv.instances[i];
        if (!inst->active)          continue;
        if (inst->epoch  != epoch)  continue;
        if (inst->screen != screen) continue;

        x = inst->x;
        y = inst->y;

        if (inst->item_id == ITEM_STICK && g_stick_sprite.data != NULL)
        {
            draw_bitmap_buf_t(&g_stick_sprite, x, y);
            continue;
        }

        /* Placeholder: rectangulo amarillo para el resto de objetos */
        draw_line(x,     y,     x + 8, y,     14);
        draw_line(x + 8, y,     x + 8, y + 8, 14);
        draw_line(x + 8, y + 8, x,     y + 8, 14);
        draw_line(x,     y + 8, x,     y,     14);
    }
}
