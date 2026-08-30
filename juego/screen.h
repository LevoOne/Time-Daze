/*
 * screen.h - Sistema de pantallas de Tempus Fugit
 */

#ifndef SCREEN_SYS_H
#define SCREEN_SYS_H

#include "game.h"

/* Datos de la pantalla actual, accesibles desde player.c */
extern ScreenData g_screen_data;

/* Spritesheet de Eric, cargado en screen_init */
extern BITMAP g_spritesheet;
extern BITMAP g_rock_sprite;
extern BITMAP g_rock_roll_sprite;
extern BITMAP g_shaman_sprite;
extern BITMAP g_bear_sprite;
extern BITMAP g_boar_sprite;
extern BITMAP g_stick_sprite;
extern BITMAP g_egg_sprite;
extern BITMAP g_egg_icon;
extern BITMAP g_cup_sprite;
extern BITMAP g_cuphoney_sprite;
extern BITMAP g_honey_sprite;
extern BITMAP g_cup_icon;
extern BITMAP g_cuphoney_icon;
extern BITMAP g_eric_head;
extern BITMAP g_fire_sprite;
extern BITMAP g_log_sprite;
extern BITMAP g_log_icon;
extern BITMAP g_reptile_sprite;
extern BITMAP g_fish_sprite;
extern BITMAP g_splash_sprite;
extern BITMAP g_frag1_dim;
extern BITMAP g_frag1_collected;
extern BITMAP g_frag2_dim;
extern BITMAP g_frag2_collected;
extern BITMAP g_frag3_dim;
extern BITMAP g_frag3_collected;
extern BITMAP g_ericfrm_sprite;

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
 * screen_apply_palette()
 *   Aplica la paleta de la pantalla actual al hardware VGA directamente.
 *   Usar al arrancar o cargar partida, cuando no hay transicion de fade.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_apply_palette(void);

/* -----------------------------------------------------------------------------------------
 * screen_draw()
 *   Dibuja el BMP de fondo de la pantalla actual en el back buffer.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_draw(void);

/* -----------------------------------------------------------------------------------------
 * screen_draw_rock()
 *   Dibuja el sprite de la roca en P1 de Prehistoria si PUZZLE_LEVER no esta resuelto.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_draw_rock(void);

void screen_inject_bear_palette(void);
void screen_inject_boar_palette(void);
void screen_inject_cup_palette(void);
void screen_inject_log_palette(void);
void screen_inject_reptile_palette(void);
void screen_inject_fish_palette(void);

/* -----------------------------------------------------------------------------------------
 * screen_draw_shaman()
 *   Dibuja el chaman animado en P1 de Prehistoria sobre el dolmen izquierdo.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void screen_draw_shaman(void);

void screen_draw_fire(void);

void screen_trigger_honey_drip(void);
void screen_stop_honey_drip(void);
void screen_draw_honey_drip(void);
void screen_draw_fire(void);

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

/* -----------------------------------------------------------------------------------------
 * screen_is_honey_dripping()
 *   Devuelve 1 si la animacion de goteo de miel esta activa, 0 si no.
 *   Usar desde logic.c para condicionar la recogida de miel con la taza.
 * -----------------------------------------------------------------------------------------*/
int screen_is_honey_dripping(void);

#endif /* SCREEN_SYS_H */
