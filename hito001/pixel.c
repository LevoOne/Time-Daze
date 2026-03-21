/* Modificación de la paleta */

#include "engine.h"

main()
{   
    int i,x,y,color;
    
    vga_set_mode13h();
    
  
    for(i=0;i<50000L;i++)               
    {
        x=rand()%SCREEN_W;
        y=rand()%SCREEN_H;
        color=rand()%NUM_COLORS;
        pixel_plot(x,y,color);
        vga_put_pixel(x,y,color);
        vga_flip();
    }

    
    vga_set_text_mode();
    
}
