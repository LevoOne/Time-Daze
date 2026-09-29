/* ================================================================
 * dialog.h - Sistema de ventanas emergentes de dialogo
 *
 * Tres modos:
 *   DIALOG_HINT   - Pista/descripcion de pantalla (cierra con ESPACIO)
 *   DIALOG_YESNO  - Confirmacion S/N (resultado via dialog_got_yes())
 *   DIALOG_TALK   - Dialogo con personaje (varias frases, ESPACIO avanza)
 *
 * Uso tipico:
 *   dialog_open(DIALOG_HINT, NULL, hints[epoch][screen]);
 *   dialog_open(DIALOG_YESNO, NULL, "Guardar partida?");
 *   dialog_open(DIALOG_TALK, &g_shaman_portrait, dialog_shaman);
 *
 * En el game loop:
 *   dialog_update();   -- consume teclas
 *   dialog_draw();     -- dibuja sobre back_buffer
 * ================================================================ */

#ifndef DIALOG_H
#define DIALOG_H

#include "engine.h"

/* ----------------------------------------------------------------
 * Tipos de dialogo
 * ---------------------------------------------------------------- */
#define DIALOG_NONE    0
#define DIALOG_HINT    1   /* pista de pantalla: una frase, cierra con ESPACIO */
#define DIALOG_YESNO   2   /* confirmacion S/N                                 */
#define DIALOG_TALK    3   /* dialogo: array de frases terminado en NULL       */

/* ----------------------------------------------------------------
 * FUNCIONES PUBLICAS
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * dialog_open()
 *   Abre una ventana emergente.
 * Entrada: type     (int)          = DIALOG_HINT / DIALOG_YESNO / DIALOG_TALK
 *          portrait (BITMAP*)      = retrato del personaje, o NULL
 *          lines    (const char**) = array de strings terminado en NULL
 *                                    (para DIALOG_HINT/YESNO pasar array de 1 elemento)
 * -----------------------------------------------------------------------------------------*/
void dialog_open(int type, BITMAP *portrait, const char **lines);

/* -----------------------------------------------------------------------------------------
 * dialog_close()
 *   Cierra la ventana emergente activa.
 * -----------------------------------------------------------------------------------------*/
void dialog_close(void);

/* -----------------------------------------------------------------------------------------
 * dialog_is_open()
 *   Devuelve 1 si hay una ventana abierta, 0 si no.
 * -----------------------------------------------------------------------------------------*/
int dialog_is_open(void);

/* -----------------------------------------------------------------------------------------
 * dialog_got_yes()
 *   Despues de un DIALOG_YESNO cerrado, devuelve 1 si el jugador eligio S.
 *   Solo valido el frame inmediatamente despues de que dialog_is_open() pase a 0.
 * -----------------------------------------------------------------------------------------*/
int dialog_got_yes(void);

/* -----------------------------------------------------------------------------------------
 * dialog_update()
 *   Procesa la entrada del jugador. Llamar una vez por tick desde timed.c.
 * -----------------------------------------------------------------------------------------*/
void dialog_update(void);

/* -----------------------------------------------------------------------------------------
 * dialog_draw()
 *   Dibuja la ventana sobre back_buffer. Llamar al final de draw_frame() en timed.c,
 *   despues del HUD, para que quede encima de todo.
 * -----------------------------------------------------------------------------------------*/
void dialog_draw(void);

/* ----------------------------------------------------------------
 * TEXTOS: hints por pantalla y dialogos de personajes
 * Definidos en dialog.c; declarados aqui para uso externo opcional.
 * ---------------------------------------------------------------- */
extern const char *g_hints[3][9][3];

extern const char *g_dialog_shaman[];
extern const char *g_dialog_monolith[];
extern const char *g_dialog_med_p3_empty[];

/* TEMP: Solo para versión demo */
extern const char *g_dialog_demo_limit[];

#endif /* DIALOG_H */
