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
#include "dialog.h"

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

    /* Colocar objetos iniciales en el mundo */
    inv_place(ITEM_STICK,    EPOCH_PREHISTORY, 2, 104, 134); /* P3: palo   */
    inv_place(ITEM_LOG,      EPOCH_PREHISTORY, 8, 240, 140); /* P9: tronco */
    inv_place(ITEM_DINO_EGG, EPOCH_PREHISTORY, 7, 264, 117); /* P8: huevo  */
    inv_place(ITEM_CUP,      EPOCH_PREHISTORY, 0, 244, 132); /* P1: taza TEMP */
}

/* ----------------------------------------------------------------
 * ERIC ACTION
 * Deteccion de flanco para evitar repeticion de accion
 * ---------------------------------------------------------------- */
int eric_action(void)
{
    if (key_pressed(KEY_SPACE))
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

/* ----------------------------------------------------------------
 * ERIC NEAR ITEM
 * Igual que eric_near(), pero con un radio mucho mas ajustado
 * (ITEM_PICKUP_DIST, no INTERACT_DIST). Pensado especificamente
 * para las zonas de recogida de objetos del suelo con codigo
 * dedicado (palo, huevo, tronco): si usaran el radio generoso de
 * INTERACT_DIST, sus zonas de recogida se solapan entre si (o con
 * un objeto soltado cerca) y la que tiene codigo especifico le
 * roba siempre el turno a RECOGIDA GENERAL, sin mirar si el
 * jugador esta realmente mas cerca de otra cosa.
 * ---------------------------------------------------------------- */
#define ITEM_PICKUP_DIST 16

static int eric_near_item(int x, int y)
{
    int ex, ey, dx, dy;

    ex = (int)g_game.player.x + PLAYER_WIDTH / 2;
    ey = (int)g_game.player.y + PLAYER_HEIGHT / 2;
    dx = ex - x;
    dy = ey - y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return (dx < ITEM_PICKUP_DIST && dy < ITEM_PICKUP_DIST);
}

/* ----------------------------------------------------------------
 * LOGIC SCREEN CHANGE
 * Cambia de pantalla y recarga enemigos.
 * Usar siempre en lugar de screen_change directo desde logic.c
 * ---------------------------------------------------------------- */
static void logic_screen_change(int dir)
{
    if (screen_change(dir))
        enemies_load(g_game.screen.current_epoch,
                     g_game.screen.current_screen);
}

/* ================================================================
 * LOGIC UPDATE - PREHISTORIA
 * ================================================================ */
static int logic_prehistory(int screen)
{
    int handled;
    int beehive_x, beehive_y;
    int honey_x,   honey_y;
    int deposit_x;
    int shaman_x,  shaman_y;
    int fragment_x, fragment_y;
    int monolith_x, monolith_y;
    int log_x,     log_y;
    int egg_x,     egg_y;
    int fire_x,    fire_y;
    int liana_x,   liana_y;
    int ex4,       ex7;


    handled    = 0;
    beehive_x  = 160; beehive_y  = 130;
    honey_x    = 200; honey_y    = 160;
    shaman_x   = 160; shaman_y   = 140;
    fragment_x = 160; fragment_y = 130;
    monolith_x = 160; monolith_y = 100;
    log_x      = 240; log_y      = 165;
    egg_x      = 264; egg_y      = 84;
    fire_x     = 160; fire_y     = 150;
    liana_x    = 40;  liana_y    = 160;

    /* --------------------------------------------------------
     * P3: liana que baja a pantalla 5
     * -------------------------------------------------------- */
    if (screen == 2)
    {
        /* Y=132 (no 159): el suelo de Prehistoria esta en y=148,
         * de pie el centro de Eric queda en y=132. El valor 159
         * esta calibrado para el suelo de Medieval/Futuro (y=175),
         * que es mas bajo en esta pantalla -con INTERACT_DIST=50
         * (valor de pruebas anterior) coincidia por pura casualidad
         * de margen, pero con 20 ya no entra nunca. */
        if (eric_near(295, 132) && eric_action())
            logic_screen_change(DIR_DOWN);
    }

    /* --------------------------------------------------------
     * P5: liana que sube a pantalla 3
     * -------------------------------------------------------- */
    if (screen == 4 && !eric_near(fire_x, fire_y))
    {
        if (eric_near(295, 132) && eric_action())
            logic_screen_change(DIR_UP);
    }

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
                screen_trigger_honey_drip();
                handled = 1;
            }
           else if (inv_is_carrying(ITEM_CUP) && screen_is_honey_dripping())
            {
                inv_transform(ITEM_CUP_HONEY);
                screen_stop_honey_drip();
                handled = 1;
            }
        }
    }

    /* --------------------------------------------------------
     * P3: pie colina, oso y palo
     * -------------------------------------------------------- */
    if (screen == 2)
    {
        /* Recoger el palo del suelo */
        if (inv_is_carrying(ITEM_NONE) &&
            inv_instance_exists(ITEM_STICK, EPOCH_PREHISTORY, screen))
        {
            if (eric_near_item(104, 134) && eric_action())
            {
                if (inv_remove_instance(ITEM_STICK, EPOCH_PREHISTORY, screen))
                {
                    inv_pick(ITEM_STICK);
                    handled = 1;
                }
            }
        }

        /* Depositar taza con miel en cualquier lugar de P3 */
        if (!puzzle_is_solved(PUZZLE_BEAR))
        {
            if (inv_is_carrying(ITEM_CUP_HONEY) && eric_action())
            {
                deposit_x = (int)g_game.player.x;
                inv_drop(EPOCH_PREHISTORY, screen, deposit_x, 132);
                puzzle_solve(PUZZLE_BEAR);
                g_enemies.enemies[0].pattern = PAT_HORIZONTAL;
                g_enemies.enemies[0].vel_x   = (deposit_x < (int)g_enemies.enemies[0].x)
                                                ? -0.2f : 0.2f;
                g_enemies.enemies[0].min_x   = (float)deposit_x;
                g_enemies.enemies[0].max_x   = (float)deposit_x;
                handled = 1;
            }
        }
    }

    /* --------------------------------------------------------
     * P1: cima, chaman y fragmento 1
     * -------------------------------------------------------- */
    if (screen == 0)
    {
        /* Bloqueo de la roca: Eric no puede pasar a la zona izquierda
         * (donde esta el chaman) hasta resolver el puzzle del tronco.
         * La roca ocupa aproximadamente x=160, bloqueamos el paso
         * cuando Eric intenta ir hacia la izquierda de x=200 */
        if (!puzzle_is_solved(PUZZLE_LEVER))
        {
            if (g_game.player.x < 160.0f)
            {
                g_game.player.x     = 160.0f;
                g_game.player.vel_x = 0.0f;
            }
        }

        /* TODO TEMP: taza en P1 para pruebas del puzzle taza/miel/oso */
        if (inv_is_carrying(ITEM_NONE) &&
            inv_instance_exists(ITEM_CUP, EPOCH_PREHISTORY, screen))
        {
            if (eric_near(244, 116) && eric_action())
            {
                if (inv_remove_instance(ITEM_CUP, EPOCH_PREHISTORY, screen))
                    inv_pick(ITEM_CUP);
                handled = 1;
            }
        }

        /* TODO TEST: gatillo temporal para probar la animacion de la roca.
         * Eliminar cuando se implemente la logica real del tronco/palanca. */
        if (!puzzle_is_solved(PUZZLE_LEVER))
        {
            if (eric_near(160, 118) && eric_action())
            {
                screen_trigger_rock_roll();
                handled = 1;
            }
        }

        if (!puzzle_is_solved(PUZZLE_SHAMAN))
        {
            if (eric_near(shaman_x, shaman_y) && eric_action())
            {
                if (inv_is_carrying(ITEM_CLAY_POT_PLANT))
                {
                    inv_transform(ITEM_NONE);
                    puzzle_solve(PUZZLE_SHAMAN);
                    handled = 1;
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
                handled = 1;
            }
        }
    }

    /* --------------------------------------------------------
     * P9: monolito y tronco
     * -------------------------------------------------------- */
    if (screen == 8)
    {
        /* Gateado por inv_is_carrying(ITEM_NONE): de lo contrario,
         * este interactuable (que aun es un TODO sin implementar)
         * se comia el ENTER en cuanto Eric estaba cerca del
         * monolito, sin mirar el inventario para nada -bloqueando
         * RECOGIDA/DEPOSITO GENERAL si llevabas algo encima, p.ej.
         * el huevo, justo en esa zona de la pantalla. */
        if (inv_is_carrying(ITEM_NONE))
        {
            if (eric_near(monolith_x, monolith_y) && eric_action())
            {
                /* TODO: mostrar simbolos del codigo en pantalla */
                handled = 1;
            }
        }

        if (inv_is_carrying(ITEM_NONE) &&
            inv_instance_exists(ITEM_LOG, EPOCH_PREHISTORY, 8))
        {
            if (eric_near_item(log_x, log_y) && eric_action())
            {
                if (inv_remove_instance(ITEM_LOG, EPOCH_PREHISTORY, 8))
                {
                    inv_pick(ITEM_LOG);
                    handled = 1;
                }
            }
        }
    }

    /* --------------------------------------------------------
     * P8: cruce del rio, huevo de dinosaurio
     * -------------------------------------------------------- */
    if (screen == 7)
    {
        if (inv_is_carrying(ITEM_NONE) &&
            inv_instance_exists(ITEM_DINO_EGG, EPOCH_PREHISTORY, 7))
        {
            if (eric_near_item(egg_x, egg_y) && eric_action())
            {
                if (inv_remove_instance(ITEM_DINO_EGG, EPOCH_PREHISTORY, 7))
                {
                    inv_pick(ITEM_DINO_EGG);
                    handled = 1;
                }
            }
        }
    }

    /* --------------------------------------------------------
     * P4: liana que baja a pantalla 7
     * -------------------------------------------------------- */
    if (screen == 3)
    {
        ex4 = (int)g_game.player.x + PLAYER_WIDTH / 2;
        if (ex4 > liana_x - INTERACT_DIST && ex4 < liana_x + INTERACT_DIST)
            if (eric_action())
                logic_screen_change(DIR_DOWN);
    }

    /* --------------------------------------------------------
     * P7: liana que sube a pantalla 4
     * -------------------------------------------------------- */
    if (screen == 6)
    {
        ex7 = (int)g_game.player.x + PLAYER_WIDTH / 2;
        if (ex7 > liana_x - INTERACT_DIST && ex7 < liana_x + INTERACT_DIST)
            if (eric_action())
                logic_screen_change(DIR_UP);
    }

    /* --------------------------------------------------------
     * P5: hoguera, punto de guardado
     * -------------------------------------------------------- */
    if (screen == 4)
    {
        if (eric_near(fire_x, fire_y) && eric_action())
        {
            static const char *confirm[] = { "Guardar la partida?", NULL };
            dialog_open(DIALOG_YESNO, NULL, confirm);
            handled = 1;
        }
        if (!dialog_is_open() && dialog_got_yes())
            save_game();
    }

    return handled;
}

/* ================================================================
 * LOGIC UPDATE - EDAD MEDIA
 * ================================================================ */
static int logic_medieval(int screen)
{
    int handled;
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
    int liana_x,    liana_y;
    int ex4,        ex7;

    handled    = 0;
    cup_x      = 180; cup_y      = 159;
    crank_x    = 80;  crank_y    = 155;
    guard_x    = 140; guard_y    = 160;
    key_x      = 140; key_y      = 168;
    forge_x    = 60;  forge_y    = 155;
    platform_x = 160; platform_y = 140;
    fragment_x = 160; fragment_y = 20;
    torch_x    = 100; torch_y    = 160;
    chapel_x   = 160; chapel_y   = 150;
    alch_x     = 200; alch_y     = 150;
    liana_x    = 40;  liana_y    = 160;

    /* --------------------------------------------------------
     * P3: escalera que baja a pantalla 5
     * -------------------------------------------------------- */
    if (screen == 2)
    {
        if (eric_near(295, 159) && eric_action())
            logic_screen_change(DIR_DOWN);
    }

    /* --------------------------------------------------------
     * P5: escalera que sube a pantalla 3
     * -------------------------------------------------------- */
    if (screen == 4 && !eric_near(chapel_x, chapel_y))
    {
        if (eric_near(295, 159) && eric_action())
            logic_screen_change(DIR_UP);
    }

    /* --------------------------------------------------------
     * P3: casa, taza en el alfeizar
     * -------------------------------------------------------- */
    if (screen == 2)
    {
        if (eric_near(cup_x, cup_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_NONE))
            {
                inv_pick(ITEM_CUP);
                handled = 1;
            }
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
                handled = 1;
            }
        }
    }

    /* --------------------------------------------------------
     * P9: torre
     * -------------------------------------------------------- */
    if (screen == 8)
    {
        if (!puzzle_is_solved(PUZZLE_GUARD))
        {
            if (eric_near(guard_x, guard_y) && eric_action())
            {
                if (inv_is_carrying(ITEM_DINO_EGG))
                {
                    inv_transform(ITEM_NONE);
                    puzzle_solve(PUZZLE_GUARD);
                    g_enemies.enemies[0].active = 0;
                    handled = 1;
                }
            }
        }

        if (puzzle_is_solved(PUZZLE_GUARD) &&
            inv_is_carrying(ITEM_NONE))
        {
            if (eric_near(key_x, key_y) && eric_action())
            {
                inv_pick(ITEM_KEY);
                handled = 1;
            }
        }

        if (inv_is_carrying(ITEM_NONE))
        {
            if (eric_near(torch_x, torch_y) && eric_action())
            {
                inv_pick(ITEM_TORCH);
                handled = 1;
            }
        }

        if (eric_near(forge_x, forge_y) && eric_action())
        {
            if (inv_is_carrying(ITEM_TORCH))
            {
                inv_transform(ITEM_TORCH_LIT);
                handled = 1;
            }
        }

        if (!puzzle_is_solved(PUZZLE_ELEVATOR))
        {
            if (eric_near(platform_x, platform_y) && eric_action())
            {
                if (inv_is_carrying(ITEM_TORCH_LIT))
                {
                    /* TODO: animacion murcielagos */
                    handled = 1;
                }
                else if (inv_is_carrying(ITEM_LEVITATOR))
                {
                    inv_transform(ITEM_NONE);
                    puzzle_solve(PUZZLE_ELEVATOR);
                    handled = 1;
                }
            }
        }

        if (puzzle_is_solved(PUZZLE_ELEVATOR) &&
            !fragment_is_collected(FRAGMENT_2))
        {
            if (eric_near(fragment_x, fragment_y) && eric_action())
            {
                fragment_collect(FRAGMENT_2);
                save_game();
                handled = 1;
            }
        }
    }

    /* --------------------------------------------------------
     * P4: escalera que baja a pantalla 7
     * -------------------------------------------------------- */
    if (screen == 3)
    {
        ex4 = (int)g_game.player.x + PLAYER_WIDTH / 2;
        if (ex4 > liana_x - INTERACT_DIST && ex4 < liana_x + INTERACT_DIST)
            if (eric_action())
                logic_screen_change(DIR_DOWN);
    }

    /* --------------------------------------------------------
     * P7: escalera que sube a pantalla 4
     * -------------------------------------------------------- */
    if (screen == 6)
    {
        ex7 = (int)g_game.player.x + PLAYER_WIDTH / 2;
        if (ex7 > liana_x - INTERACT_DIST && ex7 < liana_x + INTERACT_DIST)
            if (eric_action())
                logic_screen_change(DIR_UP);
    }

    /* --------------------------------------------------------
     * P5: capilla, punto de guardado
     * -------------------------------------------------------- */
    if (screen == 4)
    {
        if (eric_near(chapel_x, chapel_y) && eric_action())
        {
            save_game();
            handled = 1;
        }
    }

    /* --------------------------------------------------------
     * Alquimista
     * -------------------------------------------------------- */
    if (eric_near(alch_x, alch_y) && eric_action())
    {
        if (inv_is_carrying(ITEM_VOLCANIC_MIN))
        {
            inv_transform(ITEM_SEALANT);
            handled = 1;
        }
    }

    return handled;
}

/* ================================================================
 * LOGIC UPDATE - FUTURO
 * ================================================================ */
static int logic_future(int screen)
{
    int handled;
    int radiation_x, radiation_y;
    int device_x,    device_y;
    int portal_l_x,  portal_r_x, portal_y;
    int mecha_x,     mecha_y;
    int terminal_x,  terminal_y;
    int fragment_x,  fragment_y;
    int liana_x,     liana_y;
    int ex4,         ex7;

    handled     = 0;
    radiation_x = 160; radiation_y = 160;
    device_x    = 140; device_y    = 150;
    portal_l_x  = 80;  portal_r_x  = 220;
    portal_y    = 140;
    mecha_x     = 200; mecha_y     = 140;
    terminal_x  = 160; terminal_y  = 150;
    fragment_x  = 160; fragment_y  = 160;
    liana_x     = 40;  liana_y     = 160;

    /* --------------------------------------------------------
     * P3: escalera metalica que baja a pantalla 5
     * -------------------------------------------------------- */
    if (screen == 2)
    {
        if (eric_near(295, 159) && eric_action())
            logic_screen_change(DIR_DOWN);
    }

    /* --------------------------------------------------------
     * P5: escalera metalica que sube a pantalla 3
     * -------------------------------------------------------- */
    if (screen == 4 && !eric_near(terminal_x, terminal_y))
    {
        if (eric_near(295, 159) && eric_action())
            logic_screen_change(DIR_UP);
    }

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
                handled = 1;
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
                puzzle_solve(PUZZLE_PORTAL);
                handled = 1;
            }
        }

        if (puzzle_is_solved(PUZZLE_PORTAL))
        {
            if (eric_near(portal_l_x, portal_y))
                player_place(portal_r_x, portal_y);
        }

        if (puzzle_is_solved(PUZZLE_MECHA) &&
            !fragment_is_collected(FRAGMENT_3))
        {
            if (eric_near(fragment_x, fragment_y) && eric_action())
            {
                fragment_collect(FRAGMENT_3);
                save_game();
                handled = 1;
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
                handled = 1;
            }
        }
    }

    /* --------------------------------------------------------
     * P4: escalera metalica que baja a pantalla 7
     * -------------------------------------------------------- */
    if (screen == 3)
    {
        ex4 = (int)g_game.player.x + PLAYER_WIDTH / 2;
        if (ex4 > liana_x - INTERACT_DIST && ex4 < liana_x + INTERACT_DIST)
            if (eric_action())
                logic_screen_change(DIR_DOWN);
    }

    /* --------------------------------------------------------
     * P7: escalera metalica que sube a pantalla 4
     * -------------------------------------------------------- */
    if (screen == 6)
    {
        ex7 = (int)g_game.player.x + PLAYER_WIDTH / 2;
        if (ex7 > liana_x - INTERACT_DIST && ex7 < liana_x + INTERACT_DIST)
            if (eric_action())
                logic_screen_change(DIR_UP);
    }

    /* --------------------------------------------------------
     * P5: terminal, punto de guardado
     * -------------------------------------------------------- */
    if (screen == 4)
    {
        if (eric_near(terminal_x, terminal_y) && eric_action())
        {
            save_game();
            handled = 1;
        }
    }

    return handled;
}

/* ================================================================
 * LOGIC UPDATE - PUNTO DE ENTRADA PRINCIPAL
 * ================================================================ */
void logic_update(void)
{
    static int alt_was_pressed = 0;
    static const char *hint_wrap[2];
    int epoch;
    int screen;
    int next_epoch;
    int handled;
    ItemInstance *inst;

    epoch   = g_game.screen.current_epoch;
    screen  = g_game.screen.current_screen;
    handled = 0;

    /* Si hay dialogo abierto, no procesar logica de juego */
    if (dialog_is_open()) return;

    switch (epoch)
    {
        case EPOCH_PREHISTORY:
            handled = logic_prehistory(screen);
            break;
        case EPOCH_MEDIEVAL:
            handled = logic_medieval(screen);
            break;
        case EPOCH_FUTURE:
            handled = logic_future(screen);
            break;
    }

    /* --------------------------------------------------------
     * INDICADOR HUD + RECOGIDA GENERAL DE OBJETOS DEL SUELO
     * Una sola llamada a inv_find_near_player sirve para el
     * indicador del HUD y para la recogida al pulsar accion.
     * -------------------------------------------------------- */
    if (inv_is_carrying(ITEM_NONE))
    {
        inst = inv_find_near_player(epoch, screen,
                                    (int)g_game.player.x,
                                    (int)g_game.player.y);
        hud_set_nearby_item(inst != NULL ? inst->item_id : ITEM_NONE);

        if (!handled && inst != NULL && eric_action())
        {
            inv_remove_instance(inst->item_id, inst->epoch, inst->screen);
            inv_pick(inst->item_id);
            handled = 1;
        }
    }
    else
    {
        hud_set_nearby_item(ITEM_NONE);
    }

    /* --------------------------------------------------------
     * DEPOSITO GENERAL DE OBJETOS
     * Si Eric lleva un objeto, esta sobre el suelo y pulsa
     * ENTER sin interaccion especifica activa, lo deposita.
     *
     * El punto de anclaje en Y usa el alto real del sprite del
     * objeto (inv_item_height) para que quede asentado en el
     * suelo en vez de flotar o hundirse, sea cual sea su tamano.
     * Como esto hace que objetos de distinto tamano se guarden
     * en Y distintas, RECOGIDA GENERAL (arriba) no busca en un
     * unico punto fijo: usa inv_find_near_player(), que prueba
     * el punto exacto correspondiente a cada tipo de objeto
     * conocido. Si se anade un nuevo objeto recogible con
     * sprite propio, hay que anadirlo tambien ahi. */
    if (!handled &&
        g_game.player.on_ground &&
        g_game.inv.carried != ITEM_NONE &&
        eric_action())
    {
        inv_drop(epoch, screen,
                 (int)g_game.player.x + PLAYER_WIDTH / 2,
                 (int)g_game.player.y + PLAYER_HEIGHT
                     - inv_item_height(g_game.inv.carried));
    }

    /* --------------------------------------------------------
     * VIAJE TEMPORAL
     * ALT cicla entre epocas con deteccion de flanco
     * -------------------------------------------------------- */
    if (key_pressed(KEY_A))
    {
        if (!alt_was_pressed)
        {
            alt_was_pressed = 1;
            next_epoch = (epoch + 1) % EPOCH_COUNT;
            screen_travel(next_epoch);
            enemies_load(next_epoch, screen);
        }
    }
    else
    {
        alt_was_pressed = 0;
    }

    /* --------------------------------------------------------
     * PISTA DE PANTALLA
     * Solo si ninguna interaccion especifica ocurrio este frame.
     * Sin condicion sobre nearby_item: igual en todas las pantallas.
     * -------------------------------------------------------- */
    if (!handled && !dialog_is_open())
    {
        if (key_pressed(KEY_SPACE) && !action_was_pressed)
        {
            action_was_pressed = 1;
            hint_wrap[0] = g_hints[epoch][screen];
            hint_wrap[1] = NULL;
            dialog_open(DIALOG_HINT, NULL, hint_wrap);
        }
    }

    /* Reset action_was_pressed cuando ESPACIO no esta pulsado */
    if (!key_pressed(KEY_SPACE))
        action_was_pressed = 0;
}
