/* -------------------------------------------------
 * inventory.h - Sistema de inventario de Time Daze
 * ------------------------------------------------- */

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
 * Salida:  1 si habia una instancia activa y se ha quitado, 0 si no
 *          habia ninguna (NO llamar a inv_pick si devuelve 0)
 * -----------------------------------------------------------------------------------------*/
int inv_remove_instance(int item_id, int epoch, int screen);

/* -----------------------------------------------------------------------------------------
 * inv_instance_exists()
 *   Comprueba si hay una instancia activa de un objeto en una pantalla,
 *   sin consumir nada. Usar ANTES de leer eric_action() en interacciones
 *   especificas de un objeto (palo, huevo, tronco), para no robarle el
 *   turno a RECOGIDA GENERAL si el objeto ya no esta ahi.
 * Entrada: item_id, epoch, screen = identifican el objeto a comprobar
 * Salida:  1 si hay una instancia activa, 0 si no
 * -----------------------------------------------------------------------------------------*/
int inv_instance_exists(int item_id, int epoch, int screen);

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
 * inv_item_height()
 *   Devuelve el alto en pixels del sprite real de un objeto (para poder
 *   anclar su posicion al suelo correctamente al depositarlo). Si el
 *   objeto no tiene sprite propio cargado, devuelve el alto del
 *   placeholder generico (8px).
 * Entrada: item_id (int) = identificador del objeto (ITEM_*)
 * Salida:  alto en pixels
 * -----------------------------------------------------------------------------------------*/
int inv_item_height(int item_id);

/* -----------------------------------------------------------------------------------------
 * inv_find_near_player()
 *   Busca un objeto depositado cerca de Eric, probando el punto exacto
 *   en el que DEPOSITO GENERAL habria guardado cada tipo de objeto
 *   conocido (cada uno ancla su Y segun su propio alto real). Usar
 *   esto en vez de inv_get_at directo para la recogida generica.
 * Entrada: epoch, screen (int) = epoca y pantalla donde buscar
 *          px, py (int)        = posicion de Eric (esquina superior
 *                                 izquierda, g_game.player.x/y)
 * Salida:  puntero al ItemInstance encontrado, NULL si no hay ninguno
 * -----------------------------------------------------------------------------------------*/
ItemInstance *inv_find_near_player(int epoch, int screen, int px, int py);

/* -----------------------------------------------------------------------------------------
 * inv_draw()
 *   Dibuja los objetos depositados en la pantalla actual como placeholders.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void inv_draw(void);

#endif /* INVENTORY_H */
