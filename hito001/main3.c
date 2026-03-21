/*
 * main3.c - Demo basica: cargar un bitmap con efectos
 *
 */

#include "engine.h"

int main(void)
{
    int i,x,y;
    BITMAP bmp;
    
    /* Establecer modo 13 */
    vga_set_mode13h();

    timer_install();

    /* Cargamos el BMP */
    load_bmp("rocket.bmp",&bmp);

    /* Dibujamos el bitmap centrado */
    draw_bitmap(&bmp,(SCREEN_W-bmp.width) >>1,(SCREEN_H-bmp.height) >>1);

    timer_wait(100);

    while(1);

    /* dibujar el fondo */
    for(i=0;i<SCREEN_H;i++)
        memset(&VGA[SCREEN_W*i],i,SCREEN_W);
      
    timer_wait(100);

     /* draw a tiled bitmap pattern on the left */
    for(y=0;y<=SCREEN_H-bmp.height;y+=bmp.height)
      for(x=0;x<=(SCREEN_W)/2-bmp.width;x+=bmp.width)
        draw_bitmap(&bmp,x,y);

    timer_wait(100);

     /* draw a tiled transparent bitmap pattern on the right */
    for(y=0;y<=SCREEN_H-bmp.height;y+=bmp.height)
      for(x=SCREEN_W-bmp.width;x>=SCREEN_W/2;x-=bmp.width)
        draw_transparent_bitmap(&bmp,x,y);

    timer_wait(200);
    
    /* Retorno al DOS */
    vga_set_text_mode();
    timer_uninstall();
    free(bmp.data);

    return 0;
}