/*
 * beep_test.c - Prueba del PC Speaker
 *
 * Reproduce una escala de Do mayor y sale.
 *
 * Compilar:
 *   wcl386 -l=dos4g beep_test.c engine.c judas.lib
 */

#include <stdio.h>
#include "engine.h"

/* Frecuencias de la escala de Do mayor (Hz) */
static unsigned int scale[] = {
    262,  /* Do  */
    294,  /* Re  */
    330,  /* Mi  */
    349,  /* Fa  */
    392,  /* Sol */
    440,  /* La  */
    494,  /* Si  */
    523   /* Do (octava alta) */
};

int main(void)
{
    int i;

    printf("Prueba PC Speaker - escala de Do mayor\n");

    for (i = 0; i < 8; i++)
    {
        printf("Nota %d: %d Hz\n", i + 1, scale[i]);
        speaker_beep(scale[i], 200);   /* 200ms por nota */
        delay(50);                     /* small pause between notes */
    }

    /* Nota larga al final */
    speaker_beep(523, 600);

    printf("Listo.\n");
    return 0;
}