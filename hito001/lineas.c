#include "engine.h"

void main(void)
{
int x1,y1,x2,y2,index;
byte color;

vga_set_mode13h();

for  (index = 0; index < 1000; index++)
    {
    // geta random position and color and draw a line there
    x1 = rand()%640;//x-axis location of the starting point
    y1 = rand()%480;//y-axis location of the starting point
    x2 = rand()%640;//x-axis location of the end point
    y2 = rand()%480;//y-axis location of the end point
    color = rand()%16;

    /*
    _setcolor(color); //set the color of the pixel to be drawn
    _moveto(x1,y1); //move to the start of the line;
    _lineto(x2,y2); //draw the line;
    */
    draw_line(x1, y1, x2, y2, color);
    vga_flip();

   }// end for index

while(!kbhit()){}

//put the computer back into text mode

vga_set_text_mode();

}//end main