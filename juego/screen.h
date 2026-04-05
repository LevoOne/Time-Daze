/*
 * screen.h - Sistema de pantallas de Tempus Fugit
 */

#ifndef SCREEN_H
#define SCREEN_H

#include "game.h"

/* ----------------------------------------------------------------
 * PLATAFORMAS
 * ---------------------------------------------------------------- */
#define MAX_PLATFORMS   16

typedef struct {
    int x, y, w, h;
} Platform;

typedef struct {
    Platform platforms[MAX_PLATFORMS];
    int      platform_count;
} ScreenData;

/* Datos de la pantalla actual, accesibles desde player.c */
extern ScreenData g_screen_data;

/* Spritesheet de Eric, cargado en screen_init */
extern BITMAP g_spritesheet;

/* ----------------------------------------------------------------
 * FUNCIONES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * screen_init()
 *   Inicializa el sistema de pantallas y carga el spritesheet de Eric.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_init(void);

/* -----------------------------------------------------------------------------------------
 * screen_load()
 *   Carga los datos de una pantalla concreta: plataformas y BMP de fondo.
 *   Actualiza g_screen_data y g_game.screen.
 * Entrada: epoch  (int) = epoca a cargar (EPOCH_*)
 *          screen (int) = pantalla a cargar (0..8)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_load(int epoch, int screen);

/* -----------------------------------------------------------------------------------------
 * screen_draw()
 *   Dibuja el BMP de fondo de la pantalla actual en el back buffer.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_draw(void);

/* -----------------------------------------------------------------------------------------
 * screen_change()
 *   Gestiona el cambio de pantalla en una direccion con fade.
 *   Comprueba conexiones y bloqueos por puzzles.
 * Entrada: dir (int) = direccion del cambio (DIR_*)
 * Salida:  1 si hay conexion y se ha cambiado, 0 si no hay conexion
 * -----------------------------------------------------------------------------------------*/
int screen_change(int dir);

/* -----------------------------------------------------------------------------------------
 * screen_travel()
 *   Gestiona el viaje temporal a otra epoca con fade.
 *   Mantiene la misma pantalla en la nueva epoca.
 * Entrada: new_epoch (int) = epoca destino (EPOCH_*)
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_travel(int new_epoch);

/* -----------------------------------------------------------------------------------------
 * screen_get_connection()
 *   Devuelve la pantalla conectada en una direccion, teniendo en cuenta
 *   los bloqueos por puzzles.
 * Entrada: dir (int) = direccion a consultar (DIR_*)
 * Salida:  indice de pantalla conectada (0..8), o NO_SCREEN si no hay conexion
 * -----------------------------------------------------------------------------------------*/
int screen_get_connection(int dir);

#endif /* SCREEN_H */
