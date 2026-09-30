/* -------------------------------------------------------
 * logic.h - Logica de puzzles y eventos de Time Daze
 * ------------------------------------------------------- */

#ifndef LOGIC_H
#define LOGIC_H

#include "game.h"

/* ----------------------------------------------------------------
 * CONSTANTES
 * ---------------------------------------------------------------- */
#define INTERACT_DIST   20   /* distancia en pixels para interactuar */

/* ----------------------------------------------------------------
 * FUNCIONES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * logic_init()
 *   Inicializa el sistema de logica (estado de teclas, flags internos).
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void logic_init(void);

/* -----------------------------------------------------------------------------------------
 * logic_update()
 *   Comprueba y ejecuta la logica de puzzles segun la epoca y pantalla actuales.
 *   Debe llamarse una vez por tick en el game loop, antes de player_update.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void logic_update(void);

/* -----------------------------------------------------------------------------------------
 * eric_near()
 *   Comprueba si Eric esta cerca de unas coordenadas.
 * Entrada: x, y (int) = coordenadas de referencia en pixels
 * Salida:  1 si Eric esta a menos de INTERACT_DIST pixels, 0 si no
 * -----------------------------------------------------------------------------------------*/
int eric_near(int x, int y);

/* -----------------------------------------------------------------------------------------
 * eric_action()
 *   Comprueba si Eric pulsa la tecla de accion con deteccion de flanco.
 *   Evita que una sola pulsacion se repita multiples ticks.
 * Entrada: ninguna
 * Salida:  1 en el tick en que se pulsa ENTER, 0 el resto del tiempo
 * -----------------------------------------------------------------------------------------*/
int eric_action(void);

#endif /* LOGIC_H */
