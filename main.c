
#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_color.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <allegro5/allegro_primitives.h> // NO OLVIDAR AGREGAR EN EL LINKER DEL PROYECTO
#include <math.h>

float distCalc(int ax, int ay, int bx, int by);
void triangleFractal(int ax, int ay, int bx, int by, int cx, int cy, float lMin);

int main() {

   ALLEGRO_DISPLAY *display = NULL;
   
    if(!al_init()) {
       fprintf(stderr, "failed to initialize allegro!\n");
       return -1;
    } 

    display = al_create_display(1200, 800);
   
    if(!display) {
       fprintf(stderr, "failed to create display!\n");
       return -1;
    }
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitives!\n");
	return -1;
    }
    
    int ax = 350, ay = 100, bx = 100, by = 700, cx = 800, cy = 702; 
    triangleFractal(ax, ay, bx, by, cx, cy, 300);
    al_rest(5.0);
   
   return (EXIT_SUCCESS);
}


float distCalc(int ax, int ay, int bx, int by)
{
    int deltax = bx - ax, deltay = by - ay;
    return(sqrt( deltax*deltax + deltay*deltay ));
}

void triangleFractal(int ax, int ay, int bx, int by, int cx, int cy, float lMin)   
{
    
    //calculo de distancia de lados:
    float a = distCalc(ax, ay, bx, by), b = distCalc(bx, by, cx, cy), c = distCalc(cx, cy, ax, ay);
    
    printf("despues de calculo de lados\n");
    
    if ( lMin > a || lMin > b || lMin > c )
    {
        al_draw_filled_triangle(ax, ay, bx, by, cx, cy, al_map_rgb((char)ax*ay, (char)bx*by, (char)cx*cy));
        al_flip_display();
        al_rest(0.1);
    }
    else
    {
        //calculo coordenadas del centro
        int ix = (int)(( a*ax + b*bx + c*cx )/(a + b + c));
        int iy = (int)(( a*ay + b*by + c*cy )/(a + b + c));
        
        //invocacion recursiva a los tres triangulos resultantes
        triangleFractal(ix, iy, bx, by, cx, cy, lMin);   
        triangleFractal(ax, ay, ix, iy, cx, cy, lMin);   
        triangleFractal(ax, ay, bx, by, ix, iy, lMin);   
    }
}