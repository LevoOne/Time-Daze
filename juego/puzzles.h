/* ------------------------------------------------------
 * puzzles.h - Sistema de estado de puzzles de Time Daze
 * Referencia en GDD para más detalle
 * ------------------------------------------------------ */

#ifndef PUZZLES_H
#define PUZZLES_H

#include "game.h"

/* -----------------------------------------------------------------------------------------
 * puzzle_init()
 *   Inicializa todos los puzzles como no resueltos y fragmentos como no recogidos.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void puzzle_init(void);

/* -----------------------------------------------------------------------------------------
 * puzzle_is_solved()
 *   Comprueba si un puzzle esta resuelto.
 * Entrada: puzzle_id (int) = identificador del puzzle (PUZZLE_*)
 * Salida:  1 si resuelto, 0 si no
 * -----------------------------------------------------------------------------------------*/
int puzzle_is_solved(int puzzle_id);

/* -----------------------------------------------------------------------------------------
 * puzzle_solve()
 *   Marca un puzzle como resuelto.
 * Entrada: puzzle_id (int) = identificador del puzzle (PUZZLE_*)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void puzzle_solve(int puzzle_id);

/* -----------------------------------------------------------------------------------------
 * fragment_is_collected()
 *   Comprueba si un fragmento del artilugio ha sido recogido.
 * Entrada: fragment_id (int) = identificador del fragmento (FRAGMENT_*)
 * Salida:  1 si recogido, 0 si no
 * -----------------------------------------------------------------------------------------*/
int fragment_is_collected(int fragment_id);

/* -----------------------------------------------------------------------------------------
 * fragment_collect()
 *   Marca un fragmento como recogido.
 *   El guardado automatico lo gestiona main.c (Opcion B).
 * Entrada: fragment_id (int) = identificador del fragmento (FRAGMENT_*)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void fragment_collect(int fragment_id);

/* -----------------------------------------------------------------------------------------
 * game_is_complete()
 *   Comprueba si los tres fragmentos han sido recogidos.
 * Entrada: ninguna
 * Salida:  1 si el juego esta completado, 0 si no
 * -----------------------------------------------------------------------------------------*/
int game_is_complete(void);

#endif /* PUZZLES_H */
