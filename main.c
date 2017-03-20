#include <stdio.h>
#include "fractalCallback.h"
#include "parserCmdLine.h"
#include "geoLib.h"
#include "errorNbool.h"

#define CENTEROFFSET 1.3

void triangleFractal(cardPoint_t a, cardPoint_t b, cardPoint_t c, float lMin, centerCallback triangleCenter, ALLEGRO_COLOR baseColor);
void polygonFractal(polygon_t poly, float lMin, float lConstant);
int checkTriangleParams(parseTriangulo_t info);
int checkPolygonParams(parsePoligono_t info);
int checkMandelbrotParams(parseMandelbrot_t info);

int main(int argc, char** argv)
{
    parseInfo_t* info;
    int parserInfo = parseCmdLine(argc, argv, fractalCallback, (void*)info );
    
    //si hubo error, analizar que tipo de error fue, informar al usuario, y salir del programa
    if (parserInfo != NO_ERROR) 
    { 
        switch (parserInfo)
        {    
            case EMPTY_KEY:
                fprintf(stderr, "Error: clave vacia\n");
                break;
            case EMPTY_VALUE:
                fprintf(stderr, "Error: clave sin valor correspondiente");
                break;
            case INVALID_OPTION:
                fprintf(stderr, "Error: opcion invalida\n");
                break;
            case INVALID_PARAM:
                fprintf(stderr, "Error: parametro invalido\n");
                break;
            default:
                break;
        }
        return ERROR;
    }

    //si no hubo error, validar los datos ingresador por el usuario \
    dependiendo de que fractal haya seleccionado
    if((info->modo == TRIANGULO) && !(checkTriangleParams(info->triangulo)))
    {
        fprintf(stderr, "Error: triangulo demasiado grande. \
                        Intente con angulos o lStart mas chicos\n");
        return ERROR;
    }
    else if((info->modo == POLIGONO) && !(checkPolygonParams(info->poligono)))
    {
        fprintf(stderr,"Error: poligono requiere demasiadas iteraciones. \
                        Intente disminuyendo lStart, lConst o n, \
                        o aumentando lEnd\n");
        return ERROR;
    }
    else if((info->modo == MANDELBROT) && !(checkMandelbrotParams(info->mandelbrot)))
    {
        fprintf(stderr,"Error\n");
        return ERROR;
    }

   
    //inicializacion de allegro    
    ALLEGRO_DISPLAY *display = NULL;
    if(!al_init()) {
       fprintf(stderr, "failed to initialize allegro!\n");
       return -1;
    }
    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitives!\n");
	return -1;
    }
    display = al_create_display(D_WIDTH, D_HEIGHT);
    if(!display) {
       fprintf(stderr, "failed to create display!\n");
       al_shutdown_primitives_addon();
       return -1;
    }
    al_clear_to_color(al_map_rgb(255,255,255)); 
    al_flip_display();                              

    //crear el fractal correspondiente al modo
    if( info->modo == TRIANGULO)
    {
        parseTriangulo_t triInfo = info->triangulo;
        cardPoint_t a;  //vertice inferior izquierdo
        cardPoint_t b;  //vertice inferior derecho

        a.x = (D_WIDTH-triInfo.lStart)/2;   //horizontalmente, centrar el lado ab
        b.x = (D_WIDTH+triInfo.lStart)/2;
        a.y =  D_HEIGHT*0.8;    
        b.y =  a.y;                         //el arista entre los vertices a y b debe ser horizontal

        //calcular el tercer vertice del triangulo dados dos lados y sus correspondientes angulos
        cardPoint_t c = calcTriangleVertex(a, b, INRAD(triInfo.leftAngle), INRAD(triInfo.rightAngle));
        
        //generar fractal
        triangleFractal(a, b, c, triInfo.lEnd, calcTriangleIncenter, al_map_rgb(0,0,255));
        al_flip_display();  //mostrar en pantalla
    }
    else if( info->modo == POLIGONO)
    {
        parsePoligono_t polInfo = info->poligono;   //interpretar la uncion como parsePoligono_t
        polygon_t poly;
        
        poly.n = polInfo.n;             //cantidad de lados
        poly.l = polInfo.lStart;        //longitud de las aristas
        poly.center.x = D_WIDTH/2;      //ubica el centro del poligono en el centro del display
        poly.center.y = D_HEIGHT/2;
        
        //generar fractal
        polygonFractal(poly, (float)polInfo.lEnd, (float)polInfo.lConstant);
        al_flip_display();  //mostrar el pantalla
    }
    else
    //MANDELBROT
    {
        parseMandelbrot_t mandelbrot = info->mandelbrot;
    }

    while(1);   
    
    //finalizacion de allegro
    al_destroy_display(display);
    al_shutdown_primitives_addon();
    return (EXIT_SUCCESS);
}

/* checkTriangleParams
 * 
 * Controla que los parametros ingresados por el usuario no generen un fractal 
 * triangular demasiado grande. Se toma como tamanio maximo la mitad del area 
 * del display
 * 
 * Recibe:
 * parseTriangulo_t* info: puntero a estructura que contiene el input del usuario
 * 
 * Devuelve:
 * ERROR (0) si el area del triangulo es mayor que la mitad del area del display
 * NOERROR (1) si el area del triangulo es menor que la mitad del area del display
 */
int checkTriangleParams(parseTriangulo_t info)
{

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
    if ( lMin > calcDistance(a,i) || lMin > calcDistance(b,i) || lMin > calcDistance(c,i) )
    {
        return;
    }
    else
    {     
        
        //invocacion recursiva a los tres triangulos resultantes
        color = fadeColor(baseColor, triangleCenter(i,b,c));
        al_draw_filled_triangle( a.x, a.y, b.x, b.y, c.x, c.y, color);
        al_draw_triangle( a.x, a.y, b.x, b.y, c.x, c.y, white, 1);
        al_flip_display();
        triangleFractal(i, b, c, lMin, triangleCenter, baseColor);
        
//        color = fadeColor(baseColor, triangleCenter(a,i,c));
//        al_draw_filled_triangle( a.x, a.y, i.x, i.y, c.x, c.y, color);
//        al_draw_triangle(a.x, a.y, i.x, i.y, c.x, c.y, color, 2);
//        al_flip_display();
        triangleFractal(a, i, c, lMin, triangleCenter, baseColor);
        
//        color = fadeColor(baseColor, triangleCenter(a,b,i));
//        al_draw_filled_triangle( a.x, a.y, b.x, b.y, i.x, i.y, color);
//        al_draw_triangle(a.x, a.y, b.x, b.y, i.x, i.y, color, 2);
//        al_flip_display();
        triangleFractal(a, b, i, lMin, triangleCenter, baseColor);
    }
}

/* checkPolygonParams
 * 
 * Controla que los parametros ingresados por el usuario no generen un fractal
 * poligonal demasiado intrincado, de manera que se tarde en dibujarlo 
 * en su totalidad. Se toma 1.000.000 como la maxima cantidad de lineas a 
 * dibujar. 
 * Para este numero, la demora cargando el fractal en pantalla una vez 
 * finalizado, esta en el orden de los 3 segundos. Es ampliamente mayor si se 
 * dibuja cada poligono por separado mientras se calcula
 * 
 * Se denomina i a la "profundidad" del fractal. Por ejemplo, un 
 * fractal poligonal de grado 4 que es un solo cuadrado tiene una profundidad 
 * igual a 0, y uno formado por un cuadrado grande y cuatro cuadrados mas 
 * chicos en sus esquinas tiene una profundidad igual a 1.
 * 
 * i = [ (ln(lMin/l)) / (ln(lConst)) ], siendo "[]" simbolo para parte entera.
 * 
 * Llamando m a la cantidad de lineas necesarias para dibujar a un fractal de 
 * profundidad i de un poligono de grado n:
 * 
 * m = n * (1 - n^(i+1)) / (1-n)
 * 
 * Recibe:
 * parseTriangulo_t* info: puntero a estructura que contiene el input del usuario
 * Devuelve:
 * ERROR (0) si se requiere mas de 1 millon de lineas para dibujar 
 * NOERROR (1) en caso contrario
 */
int checkPolygonParams(parsePoligono_t info)
{
    unsigned long m;
//hacer la cuenta que esta especificada arriba para m
    
    return(m < 1000000);    //si m es menor que un millon, devolver 1, si es mayor o igual, devolver cero
}

/* polygonFractal
 * 
 * Funcion recursiva que dibuja un poligono y luego poligonos mas chicos 
 * centrados en los vertices del primero. Repite este procedimiento con los 
 * nuevos poligonos dibujados hasta que la longitud de los lados sea menor a un 
 * cierto valor.
 * 
 * Recibe:
 * polygon_t poly: contiene info sobre el primer poligono
 * float lMin: minimo valor que pueden tomar los lados
 * float lConstant: valor por el cual se multiplica la longitud de los lados del 
 * poligono original para obtener la longitud de los lados de los poligonos 
 * mas pequenios
 */
void polygonFractal(polygon_t poly, float lMin, float lConstant)
{
    if( poly.l < lMin)
    {
        return;
    }
    else
    {
        drawPolygon(poly, al_map_rgb(255,255,255)); 
        polygon_t pequenioPoly = poly;                  //poligono mas pequenio 
        poly.l *= CENTEROFFSET;                         //agrandar l e manera que el centro de polyPequenio se ubique un poco mas afuera de los vertices
        pequenioPoly.center = calcPolygonVertex(poly);  //ubicar el centro del nuevo poligono en el vertice del anterior
        pequenioPoly.l *= lConstant;                    // disminuir la longitud de las aristas
        for(int i = 0; i < poly.n; i++)
        {
            polygonFractal(pequenioPoly, lMin, lConstant);      //repetir procedimiento en el vertice
            pequenioPoly.center = rotatePoint(poly.center, pequenioPoly.center, 2*PI/poly.n); //ubicar el centro de pequenioPoly en el siguiente vertice de poly
        }
//        al_flip_display();
    }
}

/* checkMandelbrotParams
 * 
 * funcion que-----------------------
 * 
 * Recibe:
 * 
 * Devuelve:
 */
int checkMandelbrotParams(parseMandelbrot_t info)
{

}