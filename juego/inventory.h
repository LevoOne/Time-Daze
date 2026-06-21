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
 * inv_place()
 *   Coloca un objeto en el mapa sin que Eric lo lleve.
 *   Usar en logic_init para inicializar los objetos del mundo.
 * Entrada: item_id (int) = identificador del objeto
 *          epoch, screen = ubicacion en el mapa
 *          x, y (int)    = coordenadas en pixels
 * Salida:  1 si OK, 0 si no hay slots libres
 * -----------------------------------------------------------------------------------------*/
int inv_place(int item_id, int epoch, int screen, int x, int y);

/* -----------------------------------------------------------------------------------------
 * inv_remove_instance()
 *   Desactiva el ItemInstance de un objeto en el mapa.
 *   Llamar justo antes de inv_pick para que el objeto desaparezca del suelo.
 * Entrada: item_id, epoch, screen = identifican el objeto a desactivar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_remove_instance(int item_id, int epoch, int screen);

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
 * inv_draw()
 *   Dibuja los objetos depositados en la pantalla actual como placeholders.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_draw(void);

#endif /* INVENTORY_H */
