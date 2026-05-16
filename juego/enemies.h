/*
 * enemies.h - Sistema de enemigos de Tempus Fugit
 */

#ifndef ENEMIES_H
#define ENEMIES_H

#include "game.h"

/* ----------------------------------------------------------------
 * TIPOS DE ENEMIGO
 * ---------------------------------------------------------------- */
#define ENEMY_NONE      0
#define ENEMY_BEAR      1   /* oso (Prehistoria p3)          */
#define ENEMY_BOAR      2   /* jabali (Prehistoria p4)       */
#define ENEMY_REPTILE   3   /* reptil (Prehistoria p7)       */
#define ENEMY_FISH      4   /* pez saltarin (Prehistoria p8) */
#define ENEMY_GUARD     5   /* guardia (Edad Media)          */
#define ENEMY_DRONE     6   /* dron (Futuro)                 */
#define ENEMY_ROBOT     7   /* robot pesado (Futuro)         */
#define ENEMY_COUNT     8

/* ----------------------------------------------------------------
 * DIMENSIONES DE CADA TIPO DE ENEMIGO (w x h en pixels)
 * Usadas en draw y en check_collision
 * ---------------------------------------------------------------- */
#define ENEMY_BEAR_W     28
#define ENEMY_BEAR_H     28
#define ENEMY_BOAR_W     24
#define ENEMY_BOAR_H     20
#define ENEMY_REPTILE_W  24
#define ENEMY_REPTILE_H  20
#define ENEMY_FISH_W     12
#define ENEMY_FISH_H     10
#define ENEMY_GUARD_W    16
#define ENEMY_GUARD_H    32
#define ENEMY_DRONE_W    20
#define ENEMY_DRONE_H    12
#define ENEMY_ROBOT_W    24
#define ENEMY_ROBOT_H    32

/* Macro para obtener el ancho de un enemigo segun su tipo */
#define ENEMY_W(type) ( \
    (type)==ENEMY_BEAR    ? ENEMY_BEAR_W    : \
    (type)==ENEMY_BOAR    ? ENEMY_BOAR_W    : \
    (type)==ENEMY_REPTILE ? ENEMY_REPTILE_W : \
    (type)==ENEMY_FISH    ? ENEMY_FISH_W    : \
    (type)==ENEMY_GUARD   ? ENEMY_GUARD_W   : \
    (type)==ENEMY_DRONE   ? ENEMY_DRONE_W   : \
    (type)==ENEMY_ROBOT   ? ENEMY_ROBOT_W   : PLAYER_WIDTH)

/* Macro para obtener el alto de un enemigo segun su tipo */
#define ENEMY_H(type) ( \
    (type)==ENEMY_BEAR    ? ENEMY_BEAR_H    : \
    (type)==ENEMY_BOAR    ? ENEMY_BOAR_H    : \
    (type)==ENEMY_REPTILE ? ENEMY_REPTILE_H : \
    (type)==ENEMY_FISH    ? ENEMY_FISH_H    : \
    (type)==ENEMY_GUARD   ? ENEMY_GUARD_H   : \
    (type)==ENEMY_DRONE   ? ENEMY_DRONE_H   : \
    (type)==ENEMY_ROBOT   ? ENEMY_ROBOT_H   : PLAYER_HEIGHT)

/* ----------------------------------------------------------------
 * PATRONES DE MOVIMIENTO
 * ---------------------------------------------------------------- */
#define PAT_HORIZONTAL  0   /* patrulla izquierda-derecha    */
#define PAT_VERTICAL    1   /* salta verticalmente (peces)   */
#define PAT_FIXED       2   /* posicion fija                 */

#define MAX_ENEMIES     8

/* ----------------------------------------------------------------
 * ESTRUCTURA DE ENEMIGO
 * ---------------------------------------------------------------- */
typedef struct {
    int   type;
    int   active;
    float x, y;
    float vel_x, vel_y;
    int   pattern;
    float min_x, max_x;
    float min_y, max_y;
    int   anim_frame;
    int   anim_timer;
} Enemy;

typedef struct {
    Enemy enemies[MAX_ENEMIES];
    int   count;
} EnemyList;

/* Variable global accesible desde logic.c */
extern EnemyList g_enemies;

/* ----------------------------------------------------------------
 * FUNCIONES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * enemies_init()
 *   Inicializa la lista de enemigos vacia.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void enemies_init(void);

/* -----------------------------------------------------------------------------------------
 * enemies_load()
 *   Carga los enemigos de una pantalla y epoca concretas.
 * Entrada: epoch, screen (int) = epoca y pantalla a cargar
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void enemies_load(int epoch, int screen);

/* -----------------------------------------------------------------------------------------
 * enemies_update()
 *   Actualiza la posicion de todos los enemigos activos segun su patron.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void enemies_update(void);

/* -----------------------------------------------------------------------------------------
 * enemies_draw()
 *   Dibuja todos los enemigos activos en el back buffer.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void enemies_draw(void);

/* -----------------------------------------------------------------------------------------
 * enemies_check_collision()
 *   Comprueba si Eric colisiona con algun enemigo activo.
 * Entrada: ninguna
 * Salida:  1 si hay colision, 0 si no
 * -----------------------------------------------------------------------------------------*/
int enemies_check_collision(void);

#endif /* ENEMIES_H */