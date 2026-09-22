/*
 * game.c - Definicion de la variable global de estado del juego
 */

#include "engine.h"
#include "game.h"

GameState g_game;
int g_debug_infinite_lives = 0; /* TEMP: Metido en plan puerta trasera para depurar, ya pensaré si lo dejo */