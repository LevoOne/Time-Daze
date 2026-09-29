/*
 * tempus.c - Time Daze
 * MS-DOS Club 2026
 *
 * Compilar:
 *   wcl386 -l=dos4g tempus.c game.c player.c enemies.c puzzles.c
 *           screen.c inventory.c save.c hud.c logic.c
 *           engine.c judas.lib
 *
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include <stdlib.h>
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
#include "dialog.h"


/* ----------------------------------------------------------------
 * ESTADOS DEL JUEGO
 * ---------------------------------------------------------------- */
#define STATE_TITLE     0
#define STATE_GAME      1
#define STATE_GAMEOVER  2
#define STATE_END       3
#define STATE_MENU      4

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
static void show_presentation(void);
static void show_game_over(void);
static void show_instructions(void);
static void show_menu(void);

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
    screen_reset_session_state();

    /* El juego empieza en P0 de Prehistoria */
    /* TEMP no olvidar dejar esto en pantalla 0 de PRE */
    screen_load(EPOCH_PREHISTORY, 0);
    enemies_load(EPOCH_PREHISTORY, 0);

   /* Cargar SFX de la pantalla inicial */
    sfx_free(1);
    if (g_game.screen.current_screen == 0)
        sfx_load(SFX_ROCK_ROLL, "moverock.wav");
    else if (g_game.screen.current_screen == 2 || g_game.screen.current_screen == 3 || g_game.screen.current_screen == 6)
        sfx_load(SFX_BEAR_STEP, "woso.wav");
    else if (g_game.screen.current_screen == 7)
        sfx_load(SFX_BEAR_STEP, "splash.wav");

    /* Cargar musica de la pantalla inicial */
    if (g_game.screen.current_screen == 4)
        music_load_xm("fire.xm");
    else
        music_load_xm("PRETHEME.XM");
    music_play(0);

    player_place(286, 100);
    screen_apply_palette();
}

/* ----------------------------------------------------------------
 * DRAW FRAME
 * ---------------------------------------------------------------- */
static void draw_frame(void)
{
    screen_draw();
    screen_draw_shaman();
    screen_draw_honey_drip();
    screen_draw_fire();
    inv_draw();
    enemies_draw();
    player_draw();
    screen_draw_rock();
    screen_draw_rock_rolling();
    hud_draw();
    dialog_draw();

#ifdef DEBUG

    /* Ajustamos color de tinta según época para que se lea bien */
    {
        int iTinta;

        switch(g_game.screen.current_epoch)
        {
            case EPOCH_PREHISTORY: iTinta = 37; break;  
            case EPOCH_MEDIEVAL:   iTinta = 96; break;  
            case EPOCH_FUTURE:     iTinta = 254; break;  
        }
        
        
        draw_string("EP:", 2, 2, iTinta);
        draw_int(g_game.screen.current_epoch, 26, 2, iTinta);
        draw_string("SC:", 50, 2, iTinta);
        draw_int(g_game.screen.current_screen + 1, 74, 2, iTinta);
        draw_string("X:", 2, 12, iTinta);
        draw_int((int)g_game.player.x, 18, 12, iTinta);
        draw_string("Y:", 50, 12, iTinta);
        draw_int((int)g_game.player.y, 66, 12, iTinta);
    }

    /* Posicion de TODOS los objetos activos en la pantalla actual,
     * uno por linea, para comparar a ojo con X/Y del jugador sin
     * tener que adivinar por una captura de pantalla cual es cual. */
    {
        int di, row, ly;
        row = 0;
        for (di = 0; di < MAX_ITEM_INSTANCES; di++)
        {
            if (!g_game.inv.instances[di].active)                          continue;
            if (g_game.inv.instances[di].epoch  != g_game.screen.current_epoch)  continue;
            if (g_game.inv.instances[di].screen != g_game.screen.current_screen) continue;

            ly = 22 + row * 10;
            draw_string("ID:", 2, ly, 254);
            draw_int(g_game.inv.instances[di].item_id, 26, ly, 254);
            draw_string("IX:", 50, ly, 254);
            draw_int(g_game.inv.instances[di].x, 74, ly, 254);
            draw_string("IY:", 98, ly, 254);
            draw_int(g_game.inv.instances[di].y, 122, ly, 254);

            row++;
            if (row >= 4) break;
        }
    }
#endif

    vga_flip();
}

/* ----------------------------------------------------------------
 * GAME LOOP
 * ---------------------------------------------------------------- */
static void game_loop(void)
{
    int player_event;
    int epoch, screen;
    static const char *confirm[] = { "Terminar partida?", NULL };
    static int esc_was_pressed = 0;

    while (g_state == STATE_GAME)
    {
        epoch  = g_game.screen.current_epoch;
        screen = g_game.screen.current_screen;

        /* 1. LOGICA DE PUZZLES Y EVENTOS */
        dialog_update();
        logic_update();

        /* 2. ACTUALIZAR ENEMIGOS */
        if(!dialog_is_open())
            enemies_update();

        /* 3. ACTUALIZAR ERIC */
        player_event = player_update();

        /* 4. COLISION CON ENEMIGOS */
        if (!dialog_is_open() && enemies_check_collision())
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
                if (!screen_change(DIR_LEFT))
                    player_place(0, (int)g_game.player.y);
                else
                    enemies_load(g_game.screen.current_epoch,
                                 g_game.screen.current_screen);
                break;
            case PLAYER_RIGHT:
                if (!screen_change(DIR_RIGHT))
                    player_place(SCREEN_W - PLAYER_WIDTH - 1,
                                 (int)g_game.player.y);
                else
                    enemies_load(g_game.screen.current_epoch,
                                 g_game.screen.current_screen);
                break;
            case PLAYER_UP:
                if (!screen_change(DIR_UP))
                    player_place((int)g_game.player.x, 0);
                else
                    enemies_load(g_game.screen.current_epoch,
                                 g_game.screen.current_screen);
                break;
            case PLAYER_DOWN:
                if (!screen_change(DIR_DOWN))
                    player_place((int)g_game.player.x,
                                 SCREEN_H - PLAYER_HEIGHT - 1);
                else
                    enemies_load(g_game.screen.current_epoch,
                                 g_game.screen.current_screen);
                break;

            case PLAYER_DEAD:
                player_hit();
                player_place(20, 100);
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
        if (key_pressed(KEY_ESC) && !esc_was_pressed)
        {
            esc_was_pressed = 1;
            dialog_open(DIALOG_YESNO, &g_ericfrm_sprite, confirm);
        }
        if (!key_pressed(KEY_ESC))
            esc_was_pressed = 0;

        if (!dialog_is_open() && dialog_got_yes())
        {
            music_stop();
            sfx_free(1);
            g_state = STATE_TITLE;
        }

        /* 8. ACTUALIZAR CONTADOR DEL CRISTAL */
        if (g_hud.cristal_active)
        {
            if (!hud_cristal_update())
            {
                hud_set_mode(HUD_NORMAL);
                screen_load(g_game.screen.current_epoch,
                            g_game.screen.current_screen);
                player_place(20, 100);
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
 * Secuencia inicial antes del menú
 * ---------------------------------------------------------------- */
static void show_presentation(void)
{
    BITMAP bmp;
    int vol;

    /* Logo del concurso */
    load_bmp("contest.bmp", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    sfx_free(1);
    sfx_load(1, "CONTEST.WAV");
    sfx_set_loop(1, 0);
    sfx_play(1, 50, MIDDLE);
    vga_fade_in(16, 7);
    audio_safe_wait(150);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    /* Logo del grupo de programacion */
    load_bmp("h3logo.bmp", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
   
    sfx_free(1);

    vga_fade_in(16, 7);
    timer_wait(140);
    vga_fade_out(16, 7);
    free(bmp.data);
    timer_wait(70);

    /* Logo Time Daze */
    music_load_xm("INTRO.XM");
    music_play(0);
    load_bmp("timed.bmp", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(210);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    /* ------------------
     * Secuencia de intro
     * ------------------ */
    load_bmp("INTRO1.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(230);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO2.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO3.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO4.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO5.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO6.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO7.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    load_bmp("INTRO8.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);
    audio_safe_wait(280);
    vga_fade_out(16, 7);
    free(bmp.data);
    audio_safe_wait(70);

    /* Paramos meelodía de intro */
    for (vol = 255; vol >= 0; vol -= 15)
    {
        judas_setmusicmastervolume(SFX_FIRST, (unsigned char)vol);
        sound_update();
        timer_wait(2);
    }
    music_stop();
}


/* ----------------------------------------------------------------
 * SHOW INSTRUCTIONS
 * ---------------------------------------------------------------- */
static void show_instructions(void)
{
    BITMAP bmp;

    /* Pantalla 1 de las instrucciones: SINOPSIS */
    load_bmp("INSTR1.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (!key_pressed(KEY_SPACE));

    vga_fade_out(16, 7);
    free(bmp.data);

    /* Pantalla 2: mecánicas del juego */
    load_bmp("INSTR2.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (key_pressed(KEY_SPACE));   /* por si venia ya pulsada de la pantalla anterior */
    while (!key_pressed(KEY_SPACE));

    /* Pantalla 3: controles del juego */
    load_bmp("INSTR3.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (key_pressed(KEY_SPACE));   /* por si venia ya pulsada de la pantalla anterior */
    while (!key_pressed(KEY_SPACE));

    /* Pantalla 4: HUD (1 / 3) */
    load_bmp("INSTR4-1.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (key_pressed(KEY_SPACE));   /* por si venia ya pulsada de la pantalla anterior */
    while (!key_pressed(KEY_SPACE));

    /* Pantalla 4: HUD (2 / 3) */
    load_bmp("INSTR4-2.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (key_pressed(KEY_SPACE));   /* por si venia ya pulsada de la pantalla anterior */
    while (!key_pressed(KEY_SPACE));

    /* Pantalla 4: HUD (3 / 3) */
    load_bmp("INSTR4-3.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width)  >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (key_pressed(KEY_SPACE));   /* por si venia ya pulsada de la pantalla anterior */
    while (!key_pressed(KEY_SPACE));
    
}

/* ----------------------------------------------------------------
 * Secuencia de game over
 * ---------------------------------------------------------------- */
static void show_game_over(void)
{
    BITMAP bmp;

    /* Detenemos la melodía en curso */
    music_stop();

    load_bmp("GAMEOVER.BMP", &bmp);
    vga_clear_screen(0);
    set_palette_silent(bmp.palette);
    draw_bitmap(&bmp, (SCREEN_W - bmp.width) >> 1, (SCREEN_H - bmp.height) >> 1);
    vga_fade_in(16, 7);

    while (key_pressed(KEY_SPACE));    /* soltar, por si venia pulsada */
    while (!key_pressed(KEY_SPACE));   /* esperar pulsacion real */

    vga_fade_out(16, 7);
    free(bmp.data);
}

/* ----------------------------------------------------------------
 * SHOW MENU
 * Menu principal tras la intro. Seleccion por numero (1-4), sin
 * cursor. Dos bitmaps segun si existe partida guardada, para no
 * tener que dibujar "Cargar partida" atenuado en tiempo de
 * ejecucion: MENUS.BMP (con guardado) / MENUN.BMP (sin guardado).
 * ---------------------------------------------------------------- */
static void show_menu(void)
{
    BITMAP menu_bg;
    int    has_save;
    int    choice;
    int    done;

    done = 0;

    while (!done)
    {
        has_save = save_exists();

        load_bmp(has_save ? "MENUS.BMP" : "MENUN.BMP", &menu_bg);
        vga_clear_screen(0);
        set_palette_silent(menu_bg.palette);
        draw_bitmap(&menu_bg, (SCREEN_W - menu_bg.width) >> 1, (SCREEN_H - menu_bg.height) >> 1);
        vga_fade_in(16, 7);
        

        choice = 0;
        while (choice == 0)
        {
            if (key_pressed(KEY_1))
                choice = 1;
            else if (key_pressed(KEY_2) && has_save)
                choice = 2;
            else if (key_pressed(KEY_3))
                choice = 3;
            else if (key_pressed(KEY_4))
                choice = 4;
            else if (key_pressed(KEY_7))
                choice = 5;   /* puerta trasera: como 3, pero vidas infinitas */
        }

        /* Esperar a soltar la tecla elegida antes de continuar. Esto arregla
         * el bug de solape con las teclas de viaje entre épocas nada más empezar
         * con el primer frame de game_loop() */
        while (key_pressed(KEY_1) || key_pressed(KEY_2) || key_pressed(KEY_3) ||
            key_pressed(KEY_4) || key_pressed(KEY_7));

        free(menu_bg.data);

        switch (choice)
        {
            case 1:
                show_instructions();
                break;   /* vuelve a mostrar el menu */

            case 2:
                load_game();
                done = 1;
                break;

            case 3:
                new_game();
                done = 1;
                break;

            case 5:
                g_debug_infinite_lives = 1;
                new_game();
                done = 1;
                break;

            case 4:
                music_free();
                sfx_free(SFX_JUMP);
                sfx_free(1);
                sound_shutdown();
                engine_shutdown();
                exit(0);
                break;
        }
    }
}


int main(void)
{
    /* ----------------------------
     * Inicializar sub-sistemas 
     * ---------------------------- */
    engine_init();
    font_init();
    sound_init();
    sfx_load(SFX_JUMP, "jump.wav");
    sfx_load(SFX_PICKUP, "PICKUP.WAV");
    sfx_load(SFX_DROP,   "DROP.WAV");
    sfx_load(SFX_HURT,   "HURT.WAV");
    music_load_xm("PRETHEME.XM");

    /* ----------------------------
     * Secuencia de presentación 
     * ---------------------------- */
    show_presentation();

    /* Cargamos los assets gráficos */
    screen_init();

    do
    {
        /* Estado inicial de la partida */
        g_state = STATE_GAME;

       /* Menu principal */
        show_menu();

        /* Bucle principal del juego */
        music_play(0);
        game_loop();

        if (g_state == STATE_GAMEOVER)
            show_game_over();

    }   while (g_state == STATE_GAMEOVER || g_state == STATE_TITLE);

    /* Preparamos la salida al DOS */
    music_free();
    sfx_free(SFX_JUMP);
    sfx_free(1);

    sound_shutdown();
    engine_shutdown();

    return 0;
}
