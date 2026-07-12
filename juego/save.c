/*
 * save.c - Sistema de guardado de Time Daze
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include <stdio.h>
#include "engine.h"
#include "game.h"
#include "screen.h"
#include "player.h"
#include "save.h"





/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void save_init(void)
{
    /* Sin estado que inicializar */
}

/* ----------------------------------------------------------------
 * SAVE GAME
 * Guarda el estado actual en SAVEGAME.DAT
 * Devuelve 1 si OK, 0 si error
 * ---------------------------------------------------------------- */
int save_game(void)
{
    FILE     *fp;
    SaveData  sd;

    sd.magic        = SAVE_MAGIC;
    sd.version      = SAVE_VERSION;
    sd.puzzles      = g_game.puzzles;
    sd.inv          = g_game.inv;
    sd.epoch        = g_game.screen.current_epoch;
    sd.screen       = g_game.screen.current_screen;
    sd.player_x     = (int)g_game.player.x;
    sd.player_y     = (int)g_game.player.y;
    sd.player_lives = g_game.player.lives;

    fp = fopen(SAVE_FILE, "wb");
    if (fp == NULL) return 0;

    fwrite(&sd, sizeof(SaveData), 1, fp);
    fclose(fp);
    return 1;
}

/* ----------------------------------------------------------------
 * LOAD GAME
 * Carga el estado desde SAVEGAME.DAT
 * Devuelve 1 si OK, 0 si error o fichero invalido
 * ---------------------------------------------------------------- */
int load_game(void)
{
    FILE     *fp;
    SaveData  sd;

    fp = fopen(SAVE_FILE, "rb");
    if (fp == NULL) return 0;

    fread(&sd, sizeof(SaveData), 1, fp);
    fclose(fp);

    if (sd.magic   != SAVE_MAGIC)   return 0;
    if (sd.version != SAVE_VERSION) return 0;

    g_game.puzzles      = sd.puzzles;
    g_game.inv          = sd.inv;
    g_game.player.lives = sd.player_lives;

    screen_load(sd.epoch, sd.screen);
    screen_apply_palette();
    player_place(sd.player_x, sd.player_y);

    /* Cargar SFX de la pantalla inicial */
    if (g_game.screen.current_screen == 0)
        sfx_load(SFX_ROCK_ROLL, "moverock.wav");
    else if (g_game.screen.current_screen == 2)
        sfx_load(SFX_BEAR_STEP, "woso.wav");

    return 1;
}

/* ----------------------------------------------------------------
 * SAVE EXISTS
 * Devuelve 1 si existe un fichero de guardado
 * ---------------------------------------------------------------- */
int save_exists(void)
{
    FILE *fp;

    fp = fopen(SAVE_FILE, "rb");
    if (fp == NULL) return 0;
    fclose(fp);
    return 1;
}

/* ----------------------------------------------------------------
 * SAVE DELETE
 * Borra el fichero de guardado
 * ---------------------------------------------------------------- */
void save_delete(void)
{
    remove(SAVE_FILE);
}