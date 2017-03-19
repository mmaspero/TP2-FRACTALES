#include "geoLib.h"

#define CENTEROFFSET 1

enum modo{TRIANGULO, POLIGONO, MANDELBROT};
void triangleFractal(cardPoint_t a, cardPoint_t b, cardPoint_t c, float lMin, centerCallback triangleCenter, ALLEGRO_COLOR baseColor);
void polygonFractal(polygon_t poly, float lMin, float lConstant);
float calclMin(cardPoint_t a, cardPoint_t b, cardPoint_t c);


int main(int argc, char** argv) 
{
    ALLEGRO_DISPLAY *display = NULL;
{
    if(!al_init()) {
       fprintf(stderr, "failed to initialize allegro!\n");
       return -1;
    } 
    
    display = al_create_display(D_WIDTH, D_HEIGHT);
   
    if(!display) {
       fprintf(stderr, "failed to create display!\n");
       return -1;
    }
    al_clear_to_color(al_map_rgb(0, 255,255)); //Hace clear del backbuffer del diplay al color RGB 255,255,255 (blanco)
    al_flip_display();
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitives!\n");
	return -1;
    }
}
    
    unsigned int fractalMode = POLIGONO;
    
    if(fractalMode == TRIANGULO)
    {
        float l = 500;
        float lMin = 285;
        float anguloA = 60;
        float anguloB = 70;


        cardPoint_t a;  //vertice inferior izquierdo
        cardPoint_t b;  //vertice inferior derecho

        a.x = (D_WIDTH-l)/2;
        b.x = (D_WIDTH+l)/2;
        a.y =  D_HEIGHT*0.8;    
        b.y =  a.y;                 //el arista entre los vertices a y b debe ser horizontal


        float alpha = INRAD(anguloA);    //angulo correspondiente al vertice a
        float beta = INRAD(anguloB);     //angulo correspondiente al vertice a

        cardPoint_t c = calcTriangleVertex(a, b, alpha, beta);

        float MinlMin = calclMin(a,b,c); //minimo valor que puede tomar lMIn

        if (lMin < MinlMin)
        {
            fprintf(stderr, "Error: lMin debe ser mayor que %f. \
                            Incremente lMin o disminuya los angulos\n", MinlMin);
            return -1;
        }
        else
        {
            triangleFractal(a, b, c, lMin, calcTriangleIncenter, al_map_rgb(255,0,255));
            al_flip_display();
            while(1);
        }
    }
    else if(fractalMode == POLIGONO)
    {
        polygon_t poly;
        poly.n = 8;         //cantidad de lados del poligono
        poly.l = 200;       //longitud de las aristas del poligono
        poly.center.x = D_WIDTH/2;       //ubica el centro del poligono en el centro del display
        poly.center.y = D_HEIGHT/2;

        polygonFractal(poly, 2, 0.4);
            al_flip_display();

        while (1);

        return 0;
    }
    else
    //MANDELBROT
    {
    
    }

    return (EXIT_SUCCESS);
}

/* triangleFractal
 * 
 * Funcion recursiva que dibuja la division de un triangulo en tres trazando 
 * nuevos lados desde el centro hasta los vertices. 
 * Esta funcion se invoca a si misma recibiendo a los tres nuevos triangulos 
 * resultantes hasta que un lado sea menor a un minimo preestablecido.
 * 
 * Recibe:
 * cardPoint_t a, b, y c: los vertices del triangulo
 * 
 * float lmin: "minima" longitud de arista 
 * (nota: la funcion detiene la recursion cuando se dibuja una arista menor a
 * lmin, por lo que lmin no es en realidad el verdadero minimo)
 * 
 * cardPoint_t triangleCenter(cardPoint_t a, cardPoint_t b, cardPoint_t c0):
 * funcion que recibe los vertices del triangulo y devuelve el centro. Esta
 * funcion permite al usuario de triangleFractal() decidir que centro de 
 * triangulo quiere elegir para realizar la division
 */
void triangleFractal(cardPoint_t a, cardPoint_t b, cardPoint_t c, float lMin, centerCallback triangleCenter, ALLEGRO_COLOR baseColor)   
{   
    ALLEGRO_COLOR color;
    ALLEGRO_COLOR white = al_map_rgb(255,255,255);
    cardPoint_t i = triangleCenter(a, b, c);    //calculo del incentro
    
    //calculo de distancia de lados:        
    if ( lMin > calcDistance(b,c) || lMin > calcDistance(a,c) || lMin > calcDistance(a,b) )
    {
/*         color = fadeColor(baseColor, triangleCenter(i,b,c));
        al_draw_filled_triangle( a.x, a.y, b.x, b.y, c.x, c.y, color);
        al_flip_display();
        al_rest(0.2);
 */
        return;
    }
    else
    {           
        //invocacion recursiva a los tres triangulos resultantes
        color = fadeColor(al_map_rgb(255,0,0), triangleCenter(i,b,c));
        al_draw_filled_triangle( i.x, i.y, b.x, b.y, c.x, c.y, color);
        al_draw_triangle( i.x, i.y, b.x, b.y, c.x, c.y, white, 2);
        al_flip_display();
        triangleFractal(i, b, c, lMin, triangleCenter, baseColor);
        
        color = fadeColor(al_map_rgb(255,0,0), triangleCenter(a,i,c));
        al_draw_filled_triangle( a.x, a.y, i.x, i.y, c.x, c.y, color);
        al_draw_triangle(a.x, a.y, i.x, i.y, c.x, c.y, white, 2);
        al_flip_display();
        triangleFractal(a, i, c, lMin, triangleCenter, baseColor);
        
        color = fadeColor(al_map_rgb(255,0,0), triangleCenter(a,b,i));
        al_draw_filled_triangle( a.x, a.y, b.x, b.y, i.x, i.y, color);
        al_draw_triangle(a.x, a.y, b.x, b.y, i.x, i.y, white, 2);
        al_flip_display();
        triangleFractal(a, b, i, lMin, triangleCenter, baseColor);
    }
}

void polygonFractal(polygon_t poly, float lMin, float lConstant)
{
    if( poly.l < lMin)
    {
        return;
    }
    else
    {
        drawPolygon(poly, al_map_rgb(0,0,255)); 
        polygon_t pequenioPoly = poly;                  //poligono mas pequenio 
        poly.l *= CENTEROFFSET;                         //agrandar l e manera que el centro de polyPequenio se ubique un poco mas afuera de los vertices
        pequenioPoly.center = calcPolygonVertex(poly);  // ubicar el centro del nuevo poligono en el vertice del anterior
        pequenioPoly.l *= lConstant;                    // disminuir la longitud de las aristas
        for(int i = 0; i < poly.n; i++)
        {
            polygonFractal(pequenioPoly, lMin, lConstant);      //repetir procedimiento en el vertice
            pequenioPoly.center = rotatePoint(poly.center, pequenioPoly.center, 2*PI/poly.n); //ubicar el centro de pequenioPoly en el siguiente vertice de poly
        }
    }
}

float calclMin(cardPoint_t a, cardPoint_t b, cardPoint_t c)
{
 /*   float cSide = calcDistance(a,b);    //calcular la longitud de los tres lados
    float aSide = calcDistance(b,c);
    float bSide = calcDistance(c,a);
    
    if(cSide > bSide && cSide > aSide)  //si c es el lado mas largo
       return ((cSide + 1)/2);
    else if(bSide > aSide)              //si b es el lado mas largo
        return((bSide + 1)/2);
    else                                //si a es el lado mas largo
        return((aSide + 1)/2);
*/
    return 0;
}