/*
 * game.h - Estructuras y constantes globales de Tempus Fugit
 *
 * Este fichero es incluido por todos los sistemas del juego.
 * Define GameState y la variable global g_game.
 * C89: compatible con OpenWatcom 2.0
 */

#ifndef GAME_H
#define GAME_H

#include "engine.h"

/* ----------------------------------------------------------------
 * EPOCAS
 * ---------------------------------------------------------------- */
#define EPOCH_PREHISTORY   0
#define EPOCH_MEDIEVAL     1
#define EPOCH_FUTURE       2
#define EPOCH_COUNT        3

/* ----------------------------------------------------------------
 * PANTALLAS Y PLATAFORMAS
 * ---------------------------------------------------------------- */
#define SCREEN_COUNT       9
#define MAX_PLATFORMS      16

typedef struct {
    int x, y, w, h;
} Platform;

typedef struct {
    Platform platforms[MAX_PLATFORMS];
    int      platform_count;
} ScreenData;

/* ----------------------------------------------------------------
 * DIRECCIONES
 * ---------------------------------------------------------------- */
#define DIR_LEFT           0
#define DIR_RIGHT          1
#define DIR_UP             2
#define DIR_DOWN           3
#define DIR_COUNT          4
#define NO_SCREEN         -1

/* ----------------------------------------------------------------
 * PUZZLES
 * ---------------------------------------------------------------- */
#define PUZZLE_BEAR        0   /* Prehistoria: el oso y la miel       */
#define PUZZLE_RIVER       1   /* Prehistoria: el tronco y el rio     */
#define PUZZLE_SHAMAN      2   /* Prehistoria: la ofrenda al chaman   */
#define PUZZLE_BRIDGE      3   /* Edad Media: el puente levadizo      */
#define PUZZLE_GUARD       4   /* Edad Media: el huevo y el guardia   */
#define PUZZLE_ELEVATOR    5   /* Edad Media: la plataforma elevadora */
#define PUZZLE_RADIATION   6   /* Futuro: la zona de radiacion        */
#define PUZZLE_PORTAL      7   /* Futuro: la puerta hermetica         */
#define PUZZLE_MECHA       8   /* Futuro: el Mecha                    */
#define PUZZLE_COUNT       9

/* ----------------------------------------------------------------
 * FRAGMENTOS
 * ---------------------------------------------------------------- */
#define FRAGMENT_1         0
#define FRAGMENT_2         1
#define FRAGMENT_3         2
#define FRAGMENT_COUNT     3

/* ----------------------------------------------------------------
 * OBJETOS
 * ---------------------------------------------------------------- */
#define ITEM_NONE            0
#define ITEM_STICK           1   /* palo (Prehistoria)                */
#define ITEM_CUP             2   /* taza vacia (Edad Media)           */
#define ITEM_CUP_HONEY       3   /* taza con miel                     */
#define ITEM_LOG             4   /* tronco (Prehistoria)              */
#define ITEM_DINO_EGG        5   /* huevo de dinosaurio (Prehistoria) */
#define ITEM_CRANK           6   /* manivela (Futuro)                 */
#define ITEM_KEY             7   /* llave de la torre (Edad Media)    */
#define ITEM_TORCH           8   /* antorcha apagada (Edad Media)     */
#define ITEM_TORCH_LIT       9   /* antorcha encendida                */
#define ITEM_LEVITATOR      10   /* artilugio de levitacion (Futuro)  */
#define ITEM_CLAY_POT       11   /* recipiente de barro (Edad Media)  */
#define ITEM_CLAY_POT_PLANT 12   /* recipiente con planta             */
#define ITEM_VOLCANIC_MIN   13   /* mineral volcanico (Prehistoria)   */
#define ITEM_SEALANT        14   /* sellante (Edad Media)             */
#define ITEM_QUARTZ         15   /* cristal de cuarzo (Prehistoria)   */
#define ITEM_QUARTZ_CHARGED 16   /* cristal de cuarzo cargado         */
#define ITEM_COUNT          17

#define MAX_ITEM_INSTANCES  32

/* ----------------------------------------------------------------
 * ESTRUCTURAS
 * ---------------------------------------------------------------- */

typedef struct {
    int solved[PUZZLE_COUNT];
    int fragments[FRAGMENT_COUNT];
} PuzzleState;

typedef struct {
    int item_id;
    int epoch;
    int screen;
    int x, y;
    int active;
} ItemInstance;

typedef struct {
    int          carried;
    ItemInstance instances[MAX_ITEM_INSTANCES];
    int          instance_count;
} Inventory;

typedef struct {
    float x, y;
    float vel_x, vel_y;
    int   on_ground;
    int   facing;
    int   anim_frame;
    int   anim_timer;
    int   lives;
} Player;

typedef struct {
    int current_epoch;
    int current_screen;
} ScreenManager;

typedef struct {
    PuzzleState   puzzles;
    Inventory     inv;
    Player        player;
    ScreenManager screen;
} GameState;

/* ----------------------------------------------------------------
 * VARIABLE GLOBAL
 * Definida en game.c, accesible desde todos los sistemas
 * ---------------------------------------------------------------- */
extern GameState g_game;

/* ----------------------------------------------------------------
 * INDICES DE EFECTOS DE SONIDO
 * ---------------------------------------------------------------- */
#define SFX_JUMP   0

#endif /* GAME_H */
