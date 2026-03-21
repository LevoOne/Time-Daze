/*
 * sound_test.c - Prueba del subsistema de sonido
 *
 * Prueba en orden:
 *   1. Inicializa el sonido
 *   2. Informa si la Sound Blaster esta disponible
 *   3. Carga un efecto WAV y una pista XM
 *   4. Reproduce la musica en bucle
 *   5. Pulsa ESPACIO para el efecto, M para parar/reanudar, ESC para salir
 *
 * Necesita en el mismo directorio:
 *   - jump.wav  (cualquier WAV corto renombrado)
 *   - music.xm  (cualquier XM renombrado)
 *
 * Compilar:
 *   wcl386 -l=dos4g sound_test.c engine.c judas.lib
 */


#include <stdio.h>
#include "engine.h"

#define SFX_JUMP   0
#define KEY_M      0x32   /* scancode de la tecla M */

int main(void)
{
    int sb_ok;
    int xm_ok;
    int sfx_ok;
    int music_playing;
    int space_was_pressed;
    int m_was_pressed;

    timer_install();
    keyboard_install();

    sb_ok = sound_init();

    printf("=== Sound Test - Tempus Fugit ===\n\n");

    if (sb_ok)
        printf("Sound Blaster OK: %d Hz  dispositivo: %s\n\n",
               judas_mixrate, judas_devname[judas_device]);
    else
        printf("Sin Sound Blaster - modo NOSOUND\n\n");

    sfx_ok = sfx_load(SFX_JUMP, "jump.wav");
    printf("jump.wav : %s\n", sfx_ok ? "OK" : "ERROR");

    xm_ok = music_load_xm("music.xm");
    printf("music.xm : %s\n\n", xm_ok ? "OK" : "ERROR");

    printf("Controles:\n");
    printf("  ESPACIO : efecto de salto\n");
    printf("  M       : parar / reanudar musica\n");
    printf("  ESC     : salir\n\n");

    if (xm_ok)
    {
        music_play(0);
        music_playing = 1;
        printf("Musica iniciada.\n\n");
    }
    else
    {
        music_playing = 0;
    }

    space_was_pressed = 0;
    m_was_pressed     = 0;

    
    

    while (!key_pressed(KEY_ESC))
    {
        sound_update();

        /* ESPACIO: efecto, con deteccion de flanco para no repetir */
        if (key_pressed(KEY_SPACE))
        {
            if (!space_was_pressed)
            {
                printf("SFX: salto\n");
                sfx_play(SFX_JUMP, 64, MIDDLE);
                space_was_pressed = 1;
            }
        }
        else
        {
            space_was_pressed = 0;
        }

        /* M: alternar musica, con deteccion de flanco */
        if (key_pressed(KEY_M))
        {
            if (!m_was_pressed)
            {
                if (music_playing)
                {
                    music_stop();
                    music_playing = 0;
                    printf("Musica parada.\n");
                }
                else
                {
                    music_play(0);
                    music_playing = 1;
                    printf("Musica reanudada.\n");
                }
                m_was_pressed = 1;
            }
        }
        else
        {
            m_was_pressed = 0;
        }
    }

    sfx_free(SFX_JUMP);
    sound_shutdown();
    keyboard_uninstall();
    timer_uninstall();

    printf("\nFin del test.\n");
    return 0;
}