/*
 * logic.c - Logica de puzzles y eventos de Tempus Fugit
 *
 * Contiene la logica concreta de cada puzzle:
 * que pasa cuando Eric interactua con objetos y personajes.
 *
 * Todas las variables se declaran al inicio del bloque (C89).
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
 * ESTADO INTERNO
 * ---------------------------------------------------------------- */
static int action_was_pressed = 0;

/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void logic_init(void)
{
    action_was_pressed = 0;
}

/* ----------------------------------------------------------------
 * ERIC ACTION
 * Deteccion de flanco para evitar repeticion de accion
 * ---------------------------------------------------------------- */
int eric_action(void)
{
    if (key_pressed(KEY_ENTER))
    {
        if (!action_was_pressed)
        {
            action_was_pressed = 1;
            return 1;
        }
    }
    else
    {
        action_was_pressed = 0;
    }
    return 0;
}

/* ----------------------------------------------------------------
 * ERIC NEAR
 * Devuelve 1 si Eric esta cerca de las coordenadas indicadas
 * ---------------------------------------------------------------- */
int eric_near(int x, int y)
{
    int ex, ey, dx, dy;

    ex = (int)g_game.player.x + PLAYER_WIDTH / 2;
    ey = (int)g_game.player.y + PLAYER_HEIGHT / 2;
    dx = ex - x;
    dy = ey - y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return (dx < INTERACT_DIST && dy < INTERACT_DIST);
}

/* ================================================================
 * LOGIC UPDATE - PREHISTORIA
 * ================================================================ */
static void logic_prehistory(int screen)
{
    /* Coordenadas de todos los elementos interactivos */
    int beehive_x,  beehive_y;
    int honey_x,    honey_y;
    int shaman_x,   shaman_y;
    int fragment_x, fragment_y;
    int monolith_x, monolith_y;
    int log_x,      log_y;
    int egg_x,      egg_y;
    int fire_x,     fire_y;

    /* Pantalla 2 (indice 1): ladera, colmena */
    beehive_x  = 160; beehive_y  = 100;
    /* Pantalla 3 (indice 2): pie colina, oso */
    honey_x    = 200; honey_y    = 160;
    /* Pantalla 1 (indice 0): cima, chaman */
    shaman_x   = 160; shaman_y   = 140;
    fragment_x = 160; fragment_y = 130;
    /* Pantalla 9 (indice 8): monolito */
    monolith_x = 160; monolith_y = 100;
    log_x      = 240; log_y      = 165;
    /* Pantalla 8 (indice 7): cruce del rio */
    egg_x      = 270; egg_y      = 160;
    /* Pantalla 5 (indice 4): hoguera */
    fire_x     = 160; fire_y     = 150;

    /* --------------------------------------------------------
     * P2: ladera, colmena
     * -------------------------------------------------------- */
    if (screen == 1)
    {
        if (eric_near(beehive_x, beehive_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_STICK))
            {
                inv_transform(ITEM_NONE);
                /* TODO: activar animacion de goteo */
            }
            else if (inv_is_carrying(ITEM_CUP))
            {
                inv_transform(ITEM_CUP_HONEY);
            }
        }
    }

    /* --------------------------------------------------------
     * P3: pie colina, oso
     * -------------------------------------------------------- */
    if (screen == 2 && !puzzle_is_solved(PUZZLE_BEAR))
    {
        if (eric_near(honey_x, honey_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_CUP_HONEY))
            {
                inv_drop(EPOCH_PREHISTORY, screen, honey_x, honey_y);
                puzzle_solve(PUZZLE_BEAR);
                g_enemies.enemies[0].active = 0;
            }
        }
    }

    /* --------------------------------------------------------
     * P1: cima, chaman y fragmento 1
     * -------------------------------------------------------- */
    if (screen == 0)
    {
        if (!puzzle_is_solved(PUZZLE_SHAMAN))
        {
            if (eric_near(shaman_x, shaman_y) && eric_action())
            {
                if (inv_is_carrying(ITEM_CLAY_POT_PLANT))
                {
                    inv_transform(ITEM_NONE);
                    puzzle_solve(PUZZLE_SHAMAN);
                    /* TODO: animacion del chaman apartandose */
                }
            }
        }

        if (puzzle_is_solved(PUZZLE_SHAMAN) &&
            !fragment_is_collected(FRAGMENT_1))
        {
            if (eric_near(fragment_x, fragment_y) && eric_action())
            {
                fragment_collect(FRAGMENT_1);
                save_game();
                /* TODO: animacion de recogida del fragmento */
            }
        }
    }

    /* --------------------------------------------------------
     * P9: monolito y tronco
     * -------------------------------------------------------- */
    if (screen == 8)
    {
        if (eric_near(monolith_x, monolith_y) && eric_action())
        {
            /* TODO: mostrar simbolos del codigo en pantalla */
        }

        if (eric_near(log_x, log_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_NONE))
                inv_pick(ITEM_LOG);
        }
    }

    /* --------------------------------------------------------
     * P8: cruce del rio, huevo de dinosaurio
     * -------------------------------------------------------- */
    if (screen == 7)
    {
        if (eric_near(egg_x, egg_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_NONE))
                inv_pick(ITEM_DINO_EGG);
        }
    }

    /* --------------------------------------------------------
     * P5: hoguera, punto de guardado
     * -------------------------------------------------------- */
    if (screen == 4)
    {
        if (eric_near(fire_x, fire_y) && eric_action())
        {
            /* TODO: mostrar mensaje de confirmacion S/N */
            save_game();
        }
    }
}

/* ================================================================
 * LOGIC UPDATE - EDAD MEDIA
 * ================================================================ */
static void logic_medieval(int screen)
{
    int cup_x,      cup_y;
    int crank_x,    crank_y;
    int guard_x,    guard_y;
    int key_x,      key_y;
    int forge_x,    forge_y;
    int platform_x, platform_y;
    int fragment_x, fragment_y;
    int chapel_x,   chapel_y;
    int alch_x,     alch_y;
    int torch_x,    torch_y;

    /* P3: casa, taza */
    cup_x      = 180; cup_y      = 150;
    /* P8: puente levadizo, manivela */
    crank_x    = 80;  crank_y    = 155;
    /* P9: torre, guardia */
    guard_x    = 140; guard_y    = 160;
    key_x      = 140; key_y      = 168;
    forge_x    = 60;  forge_y    = 155;
    platform_x = 160; platform_y = 140;
    fragment_x = 160; fragment_y = 20;
    /* Antorcha apagada en la torre */
    torch_x    = 100; torch_y    = 160;
    /* P5: capilla */
    chapel_x   = 160; chapel_y   = 150;
    /* Alquimista (pantalla pendiente de definir) */
    alch_x     = 200; alch_y     = 150;

    /* --------------------------------------------------------
     * P3: casa, taza en el alfeizar
     * -------------------------------------------------------- */
    if (screen == 2)
    {
        if (eric_near(cup_x, cup_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_NONE))
                inv_pick(ITEM_CUP);
        }
    }

    /* --------------------------------------------------------
     * P8: puente levadizo
     * -------------------------------------------------------- */
    if (screen == 7 && !puzzle_is_solved(PUZZLE_BRIDGE))
    {
        if (eric_near(crank_x, crank_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_CRANK))
            {
                inv_transform(ITEM_NONE);
                puzzle_solve(PUZZLE_BRIDGE);
                /* TODO: animacion del puente bajando */
            }
        }
    }

    /* --------------------------------------------------------
     * P9: torre
     * -------------------------------------------------------- */
    if (screen == 8)
    {
        /* Depositar huevo cerca del guardia */
        if (!puzzle_is_solved(PUZZLE_GUARD))
        {
            if (eric_near(guard_x, guard_y) && eric_action())
            {
                if (inv_is_carrying(ITEM_DINO_EGG))
                {
                    inv_transform(ITEM_NONE);
                    puzzle_solve(PUZZLE_GUARD);
                    g_enemies.enemies[0].active = 0;
                    /* TODO: animacion de eclosion del huevo */
                }
            }
        }

        /* Recoger la llave */
        if (puzzle_is_solved(PUZZLE_GUARD) &&
            inv_is_carrying(ITEM_NONE))
        {
            if (eric_near(key_x, key_y) && eric_action())
                inv_pick(ITEM_KEY);
        }

        /* Recoger la antorcha apagada */
        if (inv_is_carrying(ITEM_NONE))
        {
            if (eric_near(torch_x, torch_y) && eric_action())
                inv_pick(ITEM_TORCH);
        }

        /* Encender la antorcha en la forja */
        if (eric_near(forge_x, forge_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_TORCH))
                inv_transform(ITEM_TORCH_LIT);
        }

        /* Espantar murcielagos y activar plataforma */
        if (!puzzle_is_solved(PUZZLE_ELEVATOR))
        {
            if (eric_near(platform_x, platform_y) && eric_action())
            {
                if (inv_is_carrying(ITEM_TORCH_LIT))
                {
                    /* TODO: animacion de murcielagos huyendo */
                }
                else if (inv_is_carrying(ITEM_LEVITATOR))
                {
                    inv_transform(ITEM_NONE);
                    puzzle_solve(PUZZLE_ELEVATOR);
                    /* TODO: animacion de plataforma subiendo */
                }
            }
        }

        /* Recoger el fragmento 2 */
        if (puzzle_is_solved(PUZZLE_ELEVATOR) &&
            !fragment_is_collected(FRAGMENT_2))
        {
            if (eric_near(fragment_x, fragment_y) && eric_action())
            {
                fragment_collect(FRAGMENT_2);
                save_game();
                /* TODO: animacion de recogida del fragmento */
            }
        }
    }

    /* --------------------------------------------------------
     * P5: capilla, punto de guardado
     * -------------------------------------------------------- */
    if (screen == 4)
    {
        if (eric_near(chapel_x, chapel_y) && eric_action())
        {
            /* TODO: mostrar mensaje de confirmacion S/N */
            save_game();
        }
    }

    /* --------------------------------------------------------
     * Alquimista: entrega mineral volcanico -> crea sellante
     * Pantalla pendiente de definir en el GDD
     * -------------------------------------------------------- */
    if (eric_near(alch_x, alch_y) && eric_action())
    {
        if (inv_is_carrying(ITEM_VOLCANIC_MIN))
        {
            inv_transform(ITEM_SEALANT);
            /* TODO: animacion del alquimista creando el sellante */
        }
    }
}

/* ================================================================
 * LOGIC UPDATE - FUTURO
 * ================================================================ */
static void logic_future(int screen)
{
    int radiation_x, radiation_y;
    int device_x,    device_y;
    int portal_l_x,  portal_r_x, portal_y;
    int mecha_x,     mecha_y;
    int terminal_x,  terminal_y;
    int fragment_x,  fragment_y;

    /* P8: zona de radiacion */
    radiation_x = 160; radiation_y = 160;
    /* P6: puerta hermetica y portales */
    device_x    = 140; device_y    = 150;
    portal_l_x  = 80;  portal_r_x  = 220;
    portal_y    = 140;
    /* P9: Mecha */
    mecha_x     = 200; mecha_y     = 140;
    /* P5: terminal */
    terminal_x  = 160; terminal_y  = 150;
    /* Fragmento 3 */
    fragment_x  = 160; fragment_y  = 160;

    /* --------------------------------------------------------
     * P8: zona de radiacion
     * -------------------------------------------------------- */
    if (screen == 7 && !puzzle_is_solved(PUZZLE_RADIATION))
    {
        if (eric_near(radiation_x, radiation_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_SEALANT))
            {
                inv_transform(ITEM_NONE);
                puzzle_solve(PUZZLE_RADIATION);
                /* TODO: animacion de sellado de la radiacion */
            }
        }
    }

    /* --------------------------------------------------------
     * P6: puerta hermetica y portales
     * -------------------------------------------------------- */
    if (screen == 5)
    {
        if (!puzzle_is_solved(PUZZLE_PORTAL))
        {
            if (eric_near(device_x, device_y) && eric_action())
            {
                /* TODO: mostrar teclado de codigo en pantalla */
                /* TODO: comprobar codigo introducido          */
                puzzle_solve(PUZZLE_PORTAL);
                /* TODO: animacion de activacion de portales   */
            }
        }

        /* Eric entra por el portal izquierdo */
        if (puzzle_is_solved(PUZZLE_PORTAL))
        {
            if (eric_near(portal_l_x, portal_y))
                player_place(portal_r_x, portal_y);
        }

        /* Recoger el fragmento 3 tras despejar el Mecha */
        if (puzzle_is_solved(PUZZLE_MECHA) &&
            !fragment_is_collected(FRAGMENT_3))
        {
            if (eric_near(fragment_x, fragment_y) && eric_action())
            {
                fragment_collect(FRAGMENT_3);
                save_game();
                /* TODO: secuencia de fin del juego */
            }
        }
    }

    /* --------------------------------------------------------
     * P9: Mecha
     * -------------------------------------------------------- */
    if (screen == 8)
    {
        if (eric_near(mecha_x, mecha_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_QUARTZ_CHARGED))
            {
                inv_transform(ITEM_NONE);
                hud_cristal_activate();
                hud_set_mode(HUD_MECHA);
                /* TODO: animacion de arranque del Mecha      */
                /* TODO: iniciar secuencia de conduccion      */
            }
        }
    }

    /* --------------------------------------------------------
     * P5: terminal, punto de guardado
     * -------------------------------------------------------- */
    if (screen == 4)
    {
        if (eric_near(terminal_x, terminal_y) && eric_action())
        {
            /* TODO: mostrar mensaje de confirmacion S/N */
            save_game();
        }
    }
}

/* ================================================================
 * LOGIC UPDATE - PUNTO DE ENTRADA PRINCIPAL
 * ================================================================ */
void logic_update(void)
{
    int epoch;
    int screen;
    int next_epoch;

    epoch  = g_game.screen.current_epoch;
    screen = g_game.screen.current_screen;

    switch (epoch)
    {
        case EPOCH_PREHISTORY:
            logic_prehistory(screen);
            break;
        case EPOCH_MEDIEVAL:
            logic_medieval(screen);
            break;
        case EPOCH_FUTURE:
            logic_future(screen);
            break;
    }

    /* --------------------------------------------------------
     * VIAJE TEMPORAL
     * ALT + ACCION para viajar entre epocas
     * -------------------------------------------------------- */
    if (key_pressed(KEY_ALT) && eric_action())
    {
        next_epoch = (epoch + 1) % EPOCH_COUNT;
        screen_travel(next_epoch);
        enemies_load(next_epoch, screen);
    }
}
