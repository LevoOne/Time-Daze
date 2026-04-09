/*
 * main.c - Tempus Fugit
 * MS-DOS Club 2026
 *
 * Compilar:
 *   wcl386 -l=dos4g main.c player.c enemies.c puzzles.c
 *           screen.c inventory.c save.c hud.c logic.c
 *           engine.c judas.lib
 *
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "engine.h"
#include "game.h"
#include "screen.h"
#include "player.h"
#include "enemies.h"
#include "puzzles.h"
#include "inventory.h"
#include "save.h"
#include "hud.h"
#include "logic.h"





/* ----------------------------------------------------------------
 * DEFINICION DE LA VARIABLE GLOBAL
 * ---------------------------------------------------------------- */
GameState g_game;

/* ----------------------------------------------------------------
 * ESTADOS DEL JUEGO
 * ---------------------------------------------------------------- */
#define STATE_TITLE     0
#define STATE_GAME      1
#define STATE_GAMEOVER  2
#define STATE_END       3

static int g_state = STATE_TITLE;

/* ----------------------------------------------------------------
 * INDICE DE EFECTOS DE SONIDO
 * ---------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * PROTOTIPOS
 * ---------------------------------------------------------------- */
static void new_game(void);
static void game_loop(void);
static void draw_frame(void);

/* ----------------------------------------------------------------
 * NEW GAME
 * ---------------------------------------------------------------- */
static void new_game(void)
{
    puzzle_init();
    inv_init();
    player_init();
    hud_init();
    logic_init();
    screen_load(EPOCH_PREHISTORY, 0);
    enemies_load(EPOCH_PREHISTORY, 0);
    player_place(20, 140);
}

/* ----------------------------------------------------------------
 * DRAW FRAME
 * ---------------------------------------------------------------- */
static void draw_frame(void)
{
    screen_draw();
    inv_draw();
    enemies_draw();
    player_draw();
    hud_draw();
    vga_flip();
}

/* ----------------------------------------------------------------
 * GAME LOOP
 * ---------------------------------------------------------------- */
static void game_loop(void)
{
    int player_event;
    int epoch, screen;

    while (g_state == STATE_GAME)
    {
        epoch  = g_game.screen.current_epoch;
        screen = g_game.screen.current_screen;

        /* 1. LOGICA DE PUZZLES Y EVENTOS */
        logic_update();

        /* 2. ACTUALIZAR ENEMIGOS */
        enemies_update();

        /* 3. ACTUALIZAR ERIC */
        player_event = player_update();

        /* 4. COLISION CON ENEMIGOS */
        if (enemies_check_collision())
        {
            player_hit();
            screen_load(epoch, screen);
            enemies_load(epoch, screen);
            if (g_game.player.lives <= 0)
                g_state = STATE_GAMEOVER;
        }

        /* 5. EVENTO DE ERIC */
        switch (player_event)
        {
            case PLAYER_LEFT:
            case PLAYER_RIGHT:
            case PLAYER_UP:
            case PLAYER_DOWN:
                if (screen_change(player_event))
                {
                    enemies_load(g_game.screen.current_epoch,
                                 g_game.screen.current_screen);
                }
                break;

            case PLAYER_DEAD:
                player_hit();
                player_place(20, 140);
                if (g_game.player.lives <= 0)
                    g_state = STATE_GAMEOVER;
                break;

            case PLAYER_NONE:
            default:
                break;
        }

        /* 6. COMPROBAR FIN DEL JUEGO */
        if (game_is_complete())
            g_state = STATE_END;

        /* 7. SALIDA RAPIDA */
        if (key_pressed(KEY_ESC))
            g_state = STATE_GAMEOVER;

        /* 8. ACTUALIZAR CONTADOR DEL CRISTAL */
        if (g_hud.cristal_active)
        {
            if (!hud_cristal_update())
            {
                hud_set_mode(HUD_NORMAL);
                screen_load(g_game.screen.current_epoch,
                            g_game.screen.current_screen);
                player_place(20, 140);
            }
        }

        /* 9. DIBUJAR */
        draw_frame();

        /* 10. SONIDO */
        sound_update();

        /* 11. SINCRONIZACION */
        timer_wait(1);
    }
}

/* ----------------------------------------------------------------
 * MAIN
 * ---------------------------------------------------------------- */
int main(void)
{
    engine_init();
    font_init();   
    sound_init();

    sfx_load(SFX_JUMP, "jump.wav");
    music_load_xm("music.xm");

    /* TODO: pantalla de titulo */

    if (save_exists())
        load_game();
    else
        new_game();

    g_state = STATE_GAME;
    music_play(0);

    game_loop();

    /* TODO: pantalla de fin o game over */

    music_free();
    sfx_free(SFX_JUMP);
    sound_shutdown();
    engine_shutdown();

    return 0;
}
