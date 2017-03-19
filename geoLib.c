#include "geoLib.h"

/* drawPolygon
 * 
 * funcion que dibuja las aristas de un poligono regular.
 * 
 * Recibe:
 * polygon_t poly: poligono a dibujar
 */
void drawPolygon(polygon_t poly, ALLEGRO_COLOR  baseColor)
{   
    ALLEGRO_COLOR color;        //variable que contiene el color final de cada linea
    float alpha = 2*PI/poly.n;  //angulo entre las diagonales del poligono    
    cardPoint_t vertex = calcPolygonVertex(poly); //vertice inferior izquierdo    
    cardPoint_t nextVertex;     //otro vertice del poligono
    for(int i = 1; i <= poly.n; i++)
    {   
        //hallar un vertice unido por una arista al vertex
        nextVertex = rotatePoint(poly.center, vertex, alpha);
        
        //obtener color para el punto       
        color = fadeColor(baseColor, vertex);
        
        //dibujar la arista entre vertex y nextVertex
        al_draw_line(vertex.x, vertex.y, nextVertex.x, nextVertex.y, color, 2); 
        
        // "avanzar" de vertice para dibujar la siguiente arista
        vertex = nextVertex;
    }
}

/* getPolygonVertex
 * 
 * funcion que devuelve el vertice inferior izquierdo de un poligono 
 * (NOTA: trabaja con poligonos regulares cuyo arista inferior es horizontal)
 * 
 * Recibe: 
 * polygon_t poly: poligono cuyo vertice se quiere calcular
 * 
 * Devuelve:
 * cardPoint_t: vertice inferior izquierdo de la arista inferior horizontal.
 */
cardPoint_t calcPolygonVertex(polygon_t poly)
{
    cardPoint_t vertex;
    
    int offsety = (poly.l*sin(PI/2-PI/poly.n)/(2*sin(PI/poly.n)));
    int offsetx = (poly.l/2);
    
    vertex.y = poly.center.y + offsety;
    vertex.x = poly.center.x + offsetx;
    
    return vertex;
}

/* calcTriangleIncenter
 * 
 * funcion que calcula el centro de masa o incentro (interseccion de las 
 * bisectrices) de un triangulo
 * 
 * Recibe: 
 * cardPoint_t a, b, y c: vertices del triangulo
 * 
 * Devuelve:
 * cardPoint_t: incentro del triangulo
 */
cardPoint_t calcTriangleIncenter(cardPoint_t a, cardPoint_t b, cardPoint_t c)
{
    cardPoint_t i; //incentro del triangulo
    float sc = calcDistance(a,b), sa = calcDistance(b,c), sb = calcDistance(c,a);   //calculo longitud de lados
    i.x = (int)(( sa*(a.x) + sb*(b.x) + sc*(c.x) )/(sa + sb + sc));     //formula para encontrar coordenada x del incentro
    i.y = (int)(( sa*(a.y) + sb*(b.y) + sc*(c.y) )/(sa + sb + sc));     //idem para coordenada y
    return i;   //devolver el incentro
}

/* calcTriangleVertex
 * 
 * funcion que calcula la posicion del tercer vertice de un triangulo dado los 
 * dos primeros y sus angulos
 * 
 * Recibe:
 * 
 * Devuelve:
 */
cardPoint_t calcTriangleVertex(cardPoint_t a, cardPoint_t b, float alpha, float beta)
{
    float baseLength = calcDistance(a, b);  //largo de la base del triangulo
    float leftLength = baseLength*(sin(beta)/sin(PI - beta - alpha));   //largo del lado izquierdo del triangulo
    cardPoint_t thirdVertex;
    thirdVertex.x = a.x + cos(alpha)*leftLength;
    thirdVertex.y = a.y - sin(alpha)*leftLength;
    return thirdVertex;
}

/* rotatePoint
 * 
 * funcion que calcula las coordenadas de un punto despues de una rotacion
 * 
 * Recibe
 * cardPoint_t point: punto a rotar
 * cardPoint_t center: punto que actua como eje de rotacion
 * float alpha: angulo que se quiere rotar
 * 
 * Devuelve
 * cardPoint_t point: punto luego de la rotacion
 */
cardPoint_t rotatePoint(cardPoint_t center, cardPoint_t point, float alpha)
{
    point.x -= center.x;    //"mover" el centro hacia el (0,0)
    point.y -= center.y;
    
    cardPoint_t rotation;   //va a contener al resultado de la rotacion
    rotation.x = point.y * sin(alpha) + point.x * cos(alpha);   //rotacion respecto (0,0)
    rotation.y = point.y * cos(alpha) - point.x * sin(alpha);
    
    rotation.x += center.x;  
    rotation.y += center.y;
    
    return rotation;
}

/* calcDistance
 * 
 * funcion que calcula la distancia entre dos puntos
 * 
 * Recibe:
 * cardPoint_t a y b : puntos entre los cuales hay que hallar la distancia
 * 
 * Devuelve:
 * float : distancia entre los puntos a y b
 */
float calcDistance(cardPoint_t a, cardPoint_t b)
{
    signed int deltax = b.x - a.x, deltay = b.y - a.y;
    float raiz = sqrt( deltax*deltax + deltay*deltay );
    return(raiz);
}

/* fadeColor
 * 
 * funcion que calcula el color que, si todos los pixeles fueran analizados por 
 * esta funcion, hiciera que se genere un gradual cambio de color desde negro 
 * hasta otro color de izquierda a derecha
 * 
 * Recibe:
 * ALLEGRO_COLOR baseColor: color hacia el que tiende el 'fade'
 * cardPoint_t p: coordenadas del punto analizado
 * 
 * Devuelve:
 * ALLEGRO_COLOR color resultante
 *  
 */
ALLEGRO_COLOR fadeColor(ALLEGRO_COLOR baseColor, cardPoint_t p)
{
    return (al_map_rgb( baseColor.r*255*p.x/D_WIDTH, 
                        baseColor.g*255*p.x/D_WIDTH, 
                        baseColor.b*255*p.x/D_WIDTH ));
}