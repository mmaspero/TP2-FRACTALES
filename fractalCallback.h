#ifndef fractalCallback_h
#define fractalCallback_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "errorNbool.h"

enum modo{NOMODE, TRIANGULO, POLIGONO, MANDELBROT};

typedef struct{
    int modo;       //indica el fractal del cual contiene datos
    int lStart;     //indica la longitus de la aristas inferior del triangulo inicial
    int lEnd;       //indica la minima longitud que pueden tomar las aristas
    float rightAngle;//angulo inferior derecho del triangulo inicial
    float leftAngle;//angulo inferior izquierdo del triangulo inicial
}parseTriangulo_t;
typedef struct{
    int modo;       //indica el fractal del cual contiene datos
    int lStart;     //longitud de arista del poligono inicial
    int lEnd;       //longitud minima que pueden tomar las aristas
    float lConstant;//constante por la cual disminuye la longitud de las aristas
    float n;        //grado del poligono (cantidad de aristas)
}parsePoligono_t;
typedef struct{
    int modo;       //indica el fractal del cual contiene datos
    float x0;
    float xf;
    float y0;
    float yf;
}parseMandelbrot_t;

typedef union{
    parseTriangulo_t triangulo;
    parsePoligono_t poligono;
    parseMandelbrot_t mandelbrot;
    int modo;   //permite acceder al tipo de fractal sin usar un tipo de estructura especifica
}parseInfo_t;   //union usada para intercambiar datos entre el callback y el main

typedef char * pChar;
typedef struct
{
    pChar key;
    int (*pRut) (pChar value, parseInfo_t info);
} KEY;

int fractalCallback (char * key, char * value, void * userdata);

int rutType (pChar value, parseInfo_t info);
int rutlStartEnd (pChar value, parseInfo_t info);
int rutlConstant (pChar value, parseInfo_t info);
int rutRightAngle (pChar value, parseInfo_t info);
int rutLeftAngle (pChar value, parseInfo_t info);
int rutDatosNumericos (pChar value, parseInfo_t info);
int rutError (pChar value, parseInfo_t info);
int rutValidacion (char []);

#endif /* fractalCallback_h */
