/* -------------------------------------------------
 * player.h - Sistema de Eric (jugador) en Time Daze
 * ------------------------------------------------- */

#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

/* ----------------------------------------------------------------
 * CONSTANTES DE FISICA Y ANIMACION
 * ---------------------------------------------------------------- */
#define PLAYER_WIDTH    32
#define PLAYER_HEIGHT   32
#define PLAYER_GRAVITY  0.35f
#define PLAYER_JUMP    -5.5f
#define PLAYER_SPEED    0.8f
#define ANIM_SPEED      8
#define PLAYER_LIVES    3
#define FRAMES_PER_ROW  3
#define FRAME_JUMP      4

/* ----------------------------------------------------------------
 * EVENTOS DE PLAYER UPDATE
 * ---------------------------------------------------------------- */
#define PLAYER_NONE     0   /* sin evento especial         */
#define PLAYER_LEFT     1   /* salio por la izquierda      */
#define PLAYER_RIGHT    2   /* salio por la derecha        */
#define PLAYER_UP       3   /* salio por arriba            */
#define PLAYER_DOWN     4   /* salio por abajo             */
#define PLAYER_DEAD     5   /* cayo al vacio               */

/* ----------------------------------------------------------------
 * Filas del spritesheet
 * ---------------------------------------------------------------- */
#define ROW_IDLE        0
#define ROW_WALK_LEFT   1
#define ROW_WALK_RIGHT  2
#define ROW_JUMP_RIGHT  2   /* provisional: misma fila que walk right */
#define ROW_JUMP_LEFT   1   /* provisional: misma fila que walk left  */


/* ----------------------------------------------------------------
 * VARIABLES
 * ---------------------------------------------------------------- */
int invuln_timer;   /* ticks restantes de invulnerabilidad tras perder una vida */

/* -----------------------------------------------------------------------------------------
 * player_init()
 *   Inicializa la posicion, fisica y estado de Eric.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void player_init(void);

/* -----------------------------------------------------------------------------------------
 * player_update()
 *   Actualiza la posicion de Eric segun el teclado, aplica fisica
 *   y resuelve colisiones con las plataformas de la pantalla actual.
 * Entrada: ninguna
 * Salida:  PLAYER_* segun el evento ocurrido (borde alcanzado, muerte)
 * -----------------------------------------------------------------------------------------*/
int player_update(void);

/* -----------------------------------------------------------------------------------------
 * player_draw()
 *   Dibuja a Eric en el back buffer usando el spritesheet.
 *   Aplica flip horizontal segun la direccion de movimiento.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void player_draw(void);

/* -----------------------------------------------------------------------------------------
 * player_hit()
 *   Eric recibe un impacto: pierde una vida y vuelve al inicio.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void player_hit(void);

/* -----------------------------------------------------------------------------------------
 * player_place()
 *   Reposiciona a Eric en unas coordenadas y resetea su velocidad.
 * Entrada: x, y (int) = coordenadas destino en pixels
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void player_place(int x, int y);

#endif /* PLAYER_H */
