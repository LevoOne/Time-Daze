
 /* ---------------------------------------------------------------------------------------------------------------
 *  judas_test.c - Ejemplo minimo de Judas
 *
 * Prueba en orden:
 *   1. Detecta e inicializa la Sound Blaster
 *   2. Genera un sample de beep sintetico y lo reproduce como efecto DSP
 *   3. Carga y reproduce un fichero XM
 *   4. Sale al pulsar ESC
 *
 * Necesita en el mismo directorio:
 *   - judas.lib
 *   - judas.h, judascfg.h, judaserr.h
 *   - test.xm  (cualquier fichero XM que tengas a mano)
 *
 * Compilar:
 *   wcl386 -l=dos4g judas_test.c judas.lib
 *
 * En dosbox-x.conf asegurate de tener:
 *   [sblaster]
 *   sbtype=sb16
 *   sbbase=220
 *   irq=5
 *   dma=1
 *   hdma=5
 *
 * Y en el autoexec del dosbox-x.conf:
 *   SET BLASTER=A220 I5 D1 H5 T6
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dos.h>
#include "judas.h"

/* Scancode de ESC para salir */
#define SC_ESC  0x01

/* Frecuencia de mezcla y modo */
#define MIX_RATE  22050
#define MIX_MODE  (STEREO | SIXTEENBIT)

/* Duracion del beep en samples */
#define BEEP_LENGTH  4000
#define BEEP_FREQ    880

/* ----------------------------------------------------------------
 * Lee el scancode del teclado directamente del puerto 0x60.
 * Devuelve el scancode si hay tecla pulsada, 0 si no.
 * ---------------------------------------------------------------- */
static unsigned char read_key(void)
{
    unsigned char sc;
    sc = inp(0x60);
    if (sc & 0x80) return 0;   /* tecla liberada, ignorar */
    return sc;
}

/* ----------------------------------------------------------------
 * Genera un sample de beep sintetico (onda cuadrada simple).
 * Devuelve puntero al SAMPLE_t o NULL si fallo.
 * ---------------------------------------------------------------- */
static SAMPLE_t *make_beep(void)
{
    SAMPLE_t *smp;
    signed char *data;
    int i;
    int half;

    smp = judas_allocsample(BEEP_LENGTH);
    if (!smp)
    {
        printf("Error: no se pudo reservar memoria para el sample.\n");
        return NULL;
    }

    data = (signed char *)smp->start;
    half = MIX_RATE / BEEP_FREQ / 2;
    if (half < 1) half = 1;

    /* Onda cuadrada simple: mitad positiva, mitad negativa */
    for (i = 0; i < BEEP_LENGTH; i++)
        data[i] = ((i / half) % 2 == 0) ? 80 : -80;

    /* Corregir interpolacion interna de Judas */
    judas_ipcorrect(smp);

    smp->voicemode = VM_ON | VM_ONESHOT;

    return smp;
}

/* ----------------------------------------------------------------
 * MAIN
 * ---------------------------------------------------------------- */
int main(void)
{
    SAMPLE_t *beep;
    int xm_loaded;
    unsigned char sc;

    printf("JUDAS test - OpenWatcom 2.0\n");
    printf("---------------------------\n");

    /* 1. Configuracion y deteccion de Sound Blaster */
    /*
     * judas_config() lee la variable de entorno BLASTER y rellena
     * judascfg_device, judascfg_port, judascfg_irq, judascfg_dma1.
     * Si no existe la variable, usa valores por defecto (SB en 0x220).
     */
    judas_config();

    printf("Dispositivo detectado: %s\n", judas_devname[judascfg_device]);
    printf("Puerto: 0x%X  IRQ: %d  DMA: %d\n",
           judascfg_port, judascfg_irq, judascfg_dma1);

    /* 2. Inicializacion */
    /*
     * judas_init(mixrate, mixer, mixmode, interpolation)
     *   mixrate     : frecuencia de mezcla en Hz
     *   mixer       : FASTMIXER o QUALITYMIXER
     *   mixmode     : combinacion de MONO/STEREO y EIGHTBIT/SIXTEENBIT
     *   interpolation: 0 = sin interpolacion, 1 = con interpolacion
     */
    if (!judas_init(MIX_RATE, FASTMIXER, MIX_MODE, 0))
    {
        printf("Error al inicializar Judas: %s\n",
               judas_errortext[judas_error]);
        printf("Continuando en modo NOSOUND...\n");
    }
    else
    {
        printf("Judas inicializado: %d Hz, %s, %s\n",
               judas_mixrate,
               judas_mixmodename[judas_mixmode],
               judas_mixername[FASTMIXER]);
    }

    /* 3. Efecto DSP: beep sintetico */
    printf("\nReproduciendo beep DSP...\n");

    beep = make_beep();
    if (beep)
    {
        /*
         * judas_playsample(smp, canal, frecuencia, volumen, paneo)
         *   canal    : 0..CHANNELS-1 (usamos el ultimo para efectos)
         *   frecuencia: en Hz
         *   volumen  : 0..64
         *   paneo    : LEFT(0), MIDDLE(128), RIGHT(255)
         */
        judas_playsample(beep, CHANNELS - 1, BEEP_FREQ, 64, MIDDLE);

        /* Llamar a judas_update() mientras suena */
        /* judas_update() debe llamarse regularmente para rellenar */
        /* el buffer DMA. En el engine real lo llamaremos cada tick */
        {
            int i;
            for (i = 0; i < 200; i++)
            {
                judas_update();
                delay(10);
            }
        }

        judas_stopsample(CHANNELS - 1);
        judas_freesample(beep);
        printf("Beep reproducido.\n");
    }

    /* 4. Musica XM */
    printf("\nCargando test.xm...\n");

    xm_loaded = judas_loadxm("test.xm");
    if (!xm_loaded)
    {
        printf("Error al cargar test.xm: %s\n",
               judas_errortext[judas_error]);
        printf("(Coloca cualquier fichero XM en el directorio con nombre test.xm)\n");
    }
    else
    {
        printf("XM cargado: \"%s\"\n", judas_getxmname());
        printf("Canales: %d\n", judas_getxmchannels());

        /*
         * judas_playxm(rounds)
         *   rounds = 0 : bucle infinito
         *   rounds = 1 : reproduce una vez y para
         */
        judas_playxm(0);
        printf("\nMusicando... pulsa ESC para salir.\n");

        while (1)
        {
            judas_update();

            sc = read_key();
            if (sc == SC_ESC) break;

            /* Mostrar posicion en el XM cada cierto tiempo */
            /* (solo para debug, no necesario en el juego)  */
        }

        judas_stopxm();
        judas_freexm();
    }

    /* 5. Limpieza */
    judas_uninit();
    printf("\nJudas liberado. Fin.\n");

    return 0;
}