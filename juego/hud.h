/*
 * hud.h - Sistema de HUD de Tempus Fugit
 */

#ifndef HUD_H
#define HUD_H

#include "game.h"

/* ----------------------------------------------------------------
 * CONSTANTES
 * ---------------------------------------------------------------- */
#define HUD_Y              184   /* posicion Y de la franja del HUD  */
#define HUD_H               16   /* altura de la franja en pixels    */
#define HUD_NORMAL           0   /* HUD estandar del juego           */
#define HUD_MECHA            1   /* HUD especial del Mecha           */
#define MECHA_LIVES          3   /* impactos maximos del Mecha       */
#define CRISTAL_CARGA_TICKS  (35 * 70)   /* 35 segundos a 70 Hz     */

/* ----------------------------------------------------------------
 * ESTRUCTURA DE ESTADO DEL HUD
 * ---------------------------------------------------------------- */
typedef struct {
    int mode;
    int mecha_lives;
    int cristal_ticks;
    int cristal_active;
} HudState;

extern HudState g_hud;

/* ----------------------------------------------------------------
 * FUNCIONES
 * ---------------------------------------------------------------- */

/* -----------------------------------------------------------------------------------------
 * hud_init()
 *   Inicializa el HUD en modo normal con el Mecha intacto.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void hud_init(void);

/* -----------------------------------------------------------------------------------------
 * hud_draw()
 *   Dibuja el HUD en la franja inferior del back buffer.
 *   Selecciona automaticamente HUD normal o HUD del Mecha.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void hud_draw(void);

/* -----------------------------------------------------------------------------------------
 * hud_set_mode()
 *   Cambia el modo del HUD entre normal y Mecha.
 * Entrada: mode (int) = HUD_NORMAL o HUD_MECHA
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void hud_set_mode(int mode);

/* -----------------------------------------------------------------------------------------
 * hud_mecha_hit()
 *   Registra un impacto en el Mecha, reduciendo sus vidas restantes.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void hud_mecha_hit(void);

/* -----------------------------------------------------------------------------------------
 * hud_mecha_destroyed()
 *   Comprueba si el Mecha ha sido destruido (0 vidas restantes).
 * Entrada: ninguna
 * Salida:  1 si destruido, 0 si sigue activo
 * -----------------------------------------------------------------------------------------*/
int hud_mecha_destroyed(void);

/* -----------------------------------------------------------------------------------------
 * hud_cristal_activate()
 *   Activa el contador del cristal de cuarzo cargado.
 *   Inicia la cuenta atras de CRISTAL_CARGA_TICKS ticks.
 * Entrada: ninguna
 * Salida:  ninguna
 * -----------------------------------------------------------------------------------------*/
void hud_cristal_activate(void);

/* -----------------------------------------------------------------------------------------
 * hud_cristal_update()
 *   Decrementa el contador del cristal un tick.
 *   Debe llamarse una vez por tick en el game loop.
 * Entrada: ninguna
 * Salida:  1 si el cristal sigue activo, 0 si se ha agotado
 * -----------------------------------------------------------------------------------------*/
int hud_cristal_update(void);

#endif /* HUD_H */
