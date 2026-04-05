/*
 * puzzles.c - Sistema de estado de puzzles de Tempus Fugit
 * C89: todas las variables declaradas al inicio del bloque.
 */

#include "game.h"
#include "puzzles.h"

/* ----------------------------------------------------------------
 * INIT
 * ---------------------------------------------------------------- */
void puzzle_init(void)
{
    int i;

    for (i = 0; i < PUZZLE_COUNT;   i++) g_game.puzzles.solved[i]    = 0;
    for (i = 0; i < FRAGMENT_COUNT; i++) g_game.puzzles.fragments[i] = 0;
}

/* ----------------------------------------------------------------
 * PUZZLE IS SOLVED
 * ---------------------------------------------------------------- */
int puzzle_is_solved(int puzzle_id)
{
    if (puzzle_id < 0 || puzzle_id >= PUZZLE_COUNT) return 0;
    return g_game.puzzles.solved[puzzle_id];
}

/* ----------------------------------------------------------------
 * PUZZLE SOLVE
 * ---------------------------------------------------------------- */
void puzzle_solve(int puzzle_id)
{
    if (puzzle_id < 0 || puzzle_id >= PUZZLE_COUNT) return;
    g_game.puzzles.solved[puzzle_id] = 1;
}

/* ----------------------------------------------------------------
 * FRAGMENT IS COLLECTED
 * ---------------------------------------------------------------- */
int fragment_is_collected(int fragment_id)
{
    if (fragment_id < 0 || fragment_id >= FRAGMENT_COUNT) return 0;
    return g_game.puzzles.fragments[fragment_id];
}

/* ----------------------------------------------------------------
 * FRAGMENT COLLECT
 * El guardado automatico lo gestiona main.c (Opcion B)
 * ---------------------------------------------------------------- */
void fragment_collect(int fragment_id)
{
    if (fragment_id < 0 || fragment_id >= FRAGMENT_COUNT) return;
    g_game.puzzles.fragments[fragment_id] = 1;
}

/* ----------------------------------------------------------------
 * GAME IS COMPLETE
 * ---------------------------------------------------------------- */
int game_is_complete(void)
{
    int i;

    for (i = 0; i < FRAGMENT_COUNT; i++)
        if (!g_game.puzzles.fragments[i]) return 0;
    return 1;
}
