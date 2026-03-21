/* Manipulacion de paleta y sincronización con el refresco vertical. Ejemplo de David Brackeen adaptado al engine de Tempus Fugit */

#include "engine.h"


void main()
{
  BITMAP bmp;
  int i;

  /* Inicializar subsistemas y audio */
  engine_init();
   if (!sound_init())
        printf("Sin sonido\n");

  load_bmp("mset.bmp",&bmp);          /* open the file */

  vga_set_mode13h();       /* set the video mode. */
  timer_install();

  set_palette(bmp.palette);           /* set the palette */

  draw_bitmap(&bmp,                   /* draw the bitmap centered */
    (SCREEN_W-bmp.width) >>1,
    (SCREEN_H-bmp.height) >>1);

  timer_wait(25);

  for(i=0;i<510;i++)                  /* rotate the palette at 30hz */
  {
    wait_for_retrace();
    wait_for_retrace();
    rotate_palette(bmp.palette);
  }

  timer_wait(100);

  free(bmp.data);                     /* free up memory used */
  sound_shutdown();
  engine_shutdown();

  return;
}

