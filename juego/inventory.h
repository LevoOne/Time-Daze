/*
 * inventory.h - Sistema de inventario de Tempus Fugit
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "game.h"

/* -----------------------------------------------------------------------------------------
 * inv_init()
 *   Inicializa el inventario: sin objeto llevado, sin instancias en el mapa.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_init(void);

/* -----------------------------------------------------------------------------------------
 * inv_pick()
 *   Eric recoge un objeto. Solo funciona si no lleva nada.
 * Entrada: item_id (int) = identificador del objeto (ITEM_*)
 * Salida:  1 si OK, 0 si ya llevaba un objeto
 * -----------------------------------------------------------------------------------------*/
int inv_pick(int item_id);

/* -----------------------------------------------------------------------------------------
 * inv_drop()
 *   Eric deposita el objeto que lleva en una posicion del mapa.
 * Entrada: epoch, screen (int) = epoca y pantalla destino
 *          x, y (int)          = coordenadas en pixels
 * Salida:  1 si OK, 0 si no llevaba nada o no hay slots libres
 * -----------------------------------------------------------------------------------------*/
int inv_drop(int epoch, int screen, int x, int y);

/* -----------------------------------------------------------------------------------------
 * inv_transform()
 *   Transforma el objeto actual en otro (p.ej. taza vacia -> taza con miel).
 * Entrada: new_item_id (int) = nuevo identificador de objeto (ITEM_*)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_transform(int new_item_id);

/* -----------------------------------------------------------------------------------------
 * inv_get_carried()
 *   Devuelve el objeto que lleva Eric actualmente.
 * Entrada: ninguna
 * Salida:  ITEM_* del objeto actual, ITEM_NONE si no lleva nada
 * -----------------------------------------------------------------------------------------*/
int inv_get_carried(void);

/* -----------------------------------------------------------------------------------------
 * inv_is_carrying()
 *   Comprueba si Eric lleva un objeto concreto.
 * Entrada: item_id (int) = identificador del objeto a comprobar
 * Salida:  1 si Eric lleva ese objeto, 0 si no
 * -----------------------------------------------------------------------------------------*/
int inv_is_carrying(int item_id);

/* -----------------------------------------------------------------------------------------
 * inv_get_at()
 *   Busca un objeto depositado cerca de unas coordenadas en la pantalla actual.
 * Entrada: epoch, screen (int) = epoca y pantalla donde buscar
 *          x, y (int)          = coordenadas de referencia
 * Salida:  puntero al ItemInstance encontrado, NULL si no hay ninguno cerca
 * -----------------------------------------------------------------------------------------*/
ItemInstance *inv_get_at(int epoch, int screen, int x, int y);

/* -----------------------------------------------------------------------------------------
 * inv_remove_instance()
 *   Elimina un objeto depositado del mapa.
 * Entrada: inst (ItemInstance*) = puntero al objeto a eliminar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_remove_instance(ItemInstance *inst);

/* -----------------------------------------------------------------------------------------
 * inv_draw()
 *   Dibuja los objetos depositados en la pantalla actual como placeholders.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_draw(void);

#endif /* INVENTORY_H */
