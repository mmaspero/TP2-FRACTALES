#ifndef GEOLIB_H
#define GEOLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

#define PI 3.14159
#define INRAD(x) (x*(PI/180))
#define D_WIDTH 800
#define D_HEIGHT 800

typedef struct{     //estructura que describe a un punto mediante sus coordenadas cardinales
    float x;
    float  y;
}cardPoint_t;

typedef struct{             //estructura que define a un poligono regular con una arista inferior horizontal
    cardPoint_t center;     //coordenadas del centro del poligono
    float l;                //longitud de las aristas
    unsigned int n;         //grado del poligono
}polygon_t;

typedef cardPoint_t (*centerCallback) ( cardPoint_t a, cardPoint_t b, cardPoint_t c );

void drawPolygon(polygon_t poly, ALLEGRO_COLOR color);
ALLEGRO_COLOR fadeColor(ALLEGRO_COLOR baseColor, cardPoint_t p);
cardPoint_t calcPolygonVertex(polygon_t);
cardPoint_t calcTriangleIncenter(cardPoint_t a, cardPoint_t b, cardPoint_t c);
cardPoint_t calcTriangleVertex(cardPoint_t a, cardPoint_t b, float alpha, float beta);
cardPoint_t rotatePoint(cardPoint_t center, cardPoint_t point, float alpha);
float calcDistance(cardPoint_t a, cardPoint_t b);

#endif /* GEOLIB_H */