/* ---------------------------------------------
 * save.h - Sistema de guardado de Time Daze
 * --------------------------------------------- */

#ifndef SAVE_H
#define SAVE_H

#include "game.h"

/* ----------------------------------------------------------------
 * CONSTANTES
 * ---------------------------------------------------------------- */
#define SAVE_FILE     "SAVEGAME.DAT"
#define SAVE_MAGIC    0x5446        /* 'TF' de Tempus Fugit (nombre original del juego) */
#define SAVE_VERSION  1

/* ----------------------------------------------------------------
 * ESTRUCTURA DE DATOS DE GUARDADO
 * ---------------------------------------------------------------- */
typedef struct {
    unsigned short magic;
    unsigned short version;
    PuzzleState    puzzles;
    Inventory      inv;
    int            epoch;
    int            screen;
    int            player_x;
    int            player_y;
    int            player_lives;
} SaveData;

/* ----------------------------------------------------------------
 * FUNCIONES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * save_init()
 *   Inicializa el sistema de guardado (sin estado que inicializar).
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void save_init(void);

/* -----------------------------------------------------------------------------------------
 * save_game()
 *   Guarda el estado actual del juego en SAVEGAME.DAT.
 * Entrada: ninguna
 * Salida:  1 si OK, 0 si error de escritura
 * -----------------------------------------------------------------------------------------*/
int save_game(void);

/* -----------------------------------------------------------------------------------------
 * load_game()
 *   Carga el estado del juego desde SAVEGAME.DAT.
 * Entrada: ninguna
 * Salida:  1 si OK, 0 si error o fichero invalido
 * -----------------------------------------------------------------------------------------*/
int load_game(void);

/* -----------------------------------------------------------------------------------------
 * save_exists()
 *   Comprueba si existe un fichero de guardado valido.
 * Entrada: ninguna
 * Salida:  1 si existe, 0 si no
 * -----------------------------------------------------------------------------------------*/
int save_exists(void);

/* -----------------------------------------------------------------------------------------
 * save_delete()
 *   Borra el fichero de guardado.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void save_delete(void);

#endif /* SAVE_H */
