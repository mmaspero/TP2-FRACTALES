#ifndef PARSERCMDLINE_H
#define PARSERCMDLINE_H

#include <stdio.h>
#include "errorNbool.h"

enum errors {CALLBACK_ERROR,       //valor que devuelve el callback en caso de que halle un error de contenido      
             NO_ERROR,             //si hay opciones y parametros correctos sin error en el contenido
             EMPTY_KEY,            //si hay un valor sin clave
             EMPTY_VALUE,          //si hay una clave sin valor
             INVALID_OPTION,       //si hay una opcion con contenido invalido
             INVALID_PARAM};       //si hay un parametro con contenido invalido

typedef int (*pCallback) (char *, char *, void *);

int parseCmdLine(int argc, char *argv[], pCallback p, void *userData);

#endif
