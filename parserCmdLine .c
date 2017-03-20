#include "parserCmdLine.h"

/*
parseCmdLIne:

Funcion que chequea que los argumentos enviados al programa a traves de la linea de comandos formen OPCIONES y/o PARAMETROS

OPCIONES: estan formadas por dos argumentos
	*Clave: primer argumento de una opcion. Esta seguido de un valor. Debe iniciar con el caracter '-' y debe tener al menos un caracter mas.
	*Valor: segundo argumento de una opcion. Es consecutivo a la clave. Puede contener cualquier caracter, incluso '-' en la primer posicion
PARAMETROS: estan formados por un solo argumento. Pueden comenzar con cualquier caracter exceptuando '-', ya que se confundirian con los parametros

Hay un error de forma si hay al menos un argumento que no puede clasificarse en clave, valor, o parametro. No se toma como error de forma una invocacion sin parametros ni opciones, o solo con parametros o solo con opciones.
Una vez que chequea que los argumentos cumplen con la forma correcta, se envian a la funcion callback. Esta funcion determina si hay o no errores de contenido dentro de las opciones y parametros.
De encontrar un error, ya sea de forma o de contenido, imprime el tipo de error y no sigue analizando el resto de los argumentos.

Recibe: 
int argc: cantidad de argumentos en la invocacion al programa
char* argv[]: puntero al al arreglo de strings que contiene todos los argumentos de la linea de comando,
pCallback lafuncion: puntero a la funcion callback que analiza elcontenido de los opciones y parametros
void* userData: puntero que permite intercambiar informacion entre la funcion que invoca a parseCmdLine y la funcion de callback

Devuelve:
CALLBACK_ERROR = 0        //valor que devuelve el callback en caso de que halle un error de contenido      
NO_ERROR = 1             //si hay opciones y parametros correctos sin error en el contenido
EMPTY_KEY = 2            //si hay un valor sin clave
EMPTY_VALUE = 3          //si hay una clave sin valor
INVALID_OPTION = 4       //si hay una opcion con contenido invalido
INVALID_PARAM = 5        //si hay un parametro con contenido invalido
*/

int parseCmdLine (int argc, char * argv[], pCallback lafuncion, void *userData)
{
int parseError = NO_ERROR;                   //Indica si hubo un error o no
for(int i = 1; parseError == NO_ERROR && argc > i; i++)
{
    if ((*argv[i]) == '-')		//Entra si es una opcion (parametros no pueden empezar con '-')
    {
        if( *(argv[i]+1) == '\0')	//Chequea si la clave esta vacia
        {
            parseError = EMPTY_KEY;	//indica el error en la variable que se devuelve
        }
        else if( argc <= (i+1) )	//Chequea si falta un valor correspondiente a la clave
        {
            parseError = EMPTY_VALUE;	//indica el error en la variable que se devuelve
        }
        else    //Entra si la opcion cumple con el formato (es decir, tiene clave no vacia y valor)
        {
            if( lafuncion(argv[i], argv[i+1], userData) != CALLBACK_ERROR )	//Entra si el contenido de la opcion es correcto
            {
                ++i;                    //apuntar al argumento siguiente ya que el incremento del ciclo for cubre a solo uno de los dos argumentos de una opcion
            }
            else			//Entra si hay un error de contenido en la opcion
            {
                parseError = INVALID_OPTION;	//indica el error en la variable que se devuelve
            }
        }
    }
    //Se evalua el siguiente else if solamente si se trata de un parametro
    else if( lafuncion(NULL, argv[i], userData) == CALLBACK_ERROR ) //Entra si no hay un error de contenido en el parametro 						//Entra si hay un error de contenido en el parametro
    {
        parseError = INVALID_PARAM;     //indica el error en la variable que se devuelve
    }
}
return parseError;
}
