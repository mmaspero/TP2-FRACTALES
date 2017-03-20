#include "fractalCallback.h"


int fractalCallback (char * key, char * value, void * userdata)
{
    parseInfo_t* info = (parseInfo_t*)userdata; //info: uncion que contiene el input del usuario
    KEY* pKey;
    pChar opcion;
    pChar error = "error";
    
    //Arreglo con las posibles keys y las correspondientes rutinas de verificacion del valor de las mismas
    KEY arr[] = {
        {"type", rutType},
        {"lStart", rutlStartEnd},
        {"lEnd", rutlStartEnd},
        {"lConstant", rutlConstant},
        {"leftAngle", rutAngle},
        {"rightAngle", rutAngle},
        {"x0", rutDatosNumericos},
        {"xf", rutDatosNumericos},
        {"y0", rutDatosNumericos},
        {"yf", rutDatosNumericos},
        {"error", rutError} //en caso de que se haya ingresado una key invalida
    };
    
    opcion = key;
    pKey= arr;
    
    //Recorre el arreglo de estructuras buscando con cual coincide la opcion ingresada
    
    while ((pKey -> key != opcion) && (pKey -> key != error))   
           ++pKey;   
    
    return ((*pKey -> pRut )(value, *info));//Ejecuta rutina de accion corresondiente y devuelve lo mismo
}

// rutinas para cada key
int rutlConstant (pChar value, parseInfo_t info)
{
    int validarnumeros;
    int ok= true;
    validarnumeros = rutValidacion (value);  //analiza que el string ingresado sea todo de numeros
    if (validarnumeros == false)
    {
        ok=false;
        printf("Error \n");
    }
    else if (atof (value) >= 1 || atof (value) <= 0)
    {
        ok= false;
        printf("Error \n");
    }
    return ok;
}

/* rutlStart
 *
 * funcion que indica si el string recibido corresponde a una longitud de lado valida
 * (0 a 100). Si el string no contiene un numero o el numero se excede del rango
 * permitido, se informa al usuario. Si por el contrario, el numero es valido,
 * guarda en la estructura de comunicacion con el main siempre y cuando no haya
 * nada guardado en su lugar
 *
 * Recibe:
 * value: puntero a string a analizar
 * parseInfo_t info: estructura para almacenar datos
 *
 * Devuelve:
 * true (1) si el valor del string es valido
 * falso (0) si el string o su valor es invalido
 */


int rutlStart (pChar value, parseInfo_t info)
{
    
    if(info.modo == MANDELBROT || info.modo == NOMODE) //si se establecio un modo incompatible con lStart
    {
        fprintf(stderr, "Error, solo puede invocarse -lStart una vez establecido el modo \
                en triangulo con '-type UNIFORME'\
                o en poligono con '-type POLIGONO'\n");
        return false;
    }
    else if (info.poligono.lStart)//si lStart no esta vacio (funciona tanto para parsePoligono_t\
        como para parseTriangulo_t porque lStart se encuentra en la\
        misma posicion en ambas estructuras)
    {
        fprintf(stderr,"Error: -lStart invocado mas de una vez\n");
        return false;
    }
    
    else //si se establecio un modo compatible y lStart esta vacio
    {
        if (!rutValidacion(value)) //si se ingreso un valor no numerico
        {
            fprintf(stderr, "Error: angulo %s* con valor no numerico\n", value);
            return false;
        }
        else //si se ingreso un valor numerico
        {
            float length = atof(value);
            if ( length >= 100 || length <= 0) //si el valor numerico esta fuera de rango
            {
                fprintf(stderr, "Error: lStart %s no se encuentra en el rango indicado (0 a 100)", value);
                return false;
            }
            else    //si el valor ingresado es correcto
            {
                info.poligono.lStart = length; //nuevamente, funciona tambien para modo uniforme
                return true;
            }
        }
    }
}


int rutlEnd (pChar value, parseInfo_t info)
{
    if(info.modo = MANDELBROT || info.modo = NOMODE) //si se establecio un modo incompatible con lEnd
    {
        fprintf(stderr, "Error, solo puede invocarse -lEnd una vez establecido el modo \
                        en triangulo con '-type UNIFORME'\
                        o en poligono con '-type POLIGONO'\n");
        return false;
    }
    else if (info.poligono.lEnd)//si lEnd no esta vacio (funciona tanto para parsePoligono_t\
                                como para parseTriangulo_t porque lEnd se encuentra en la\
                                misma posicion en ambas estructuras)
    {
        fprintf(stderr,"Error: -lEnd invocado mas de una vez\n");
        return false;
    }   
    else //si se establecio un modo compatible y lEnd esta vacio    
    {
        if (!rutValidacion(value)) //si se ingreso un valor no numerico
        {
            fprintf(stderr, "Error: angulo %s* con valor no numerico\n", value);
            return false;
        }
        else //si se ingreso un valor numerico
        {
            float length = atof(value);
            if ( length > 100 || length <= 0) //si el valor numerico esta fuera de rango
            {
                fprintf(stderr, "Error: lEnd %s no se encuentra en el rango indicado (0 a 100)", value);
                return false; 
            }
            else    //si el valor ingresado es correcto
            {
                info.poligono.lEnd = length; //nuevamente, funciona tambien para modo uniforme
            }
        }
    }  
}

/* rutLeftAngle
 * 
 * funcion que indica si el string recibido corresponde a un angulo valido (0 a 
 * 90 grados). Si el string no contiene un numero o el numero se excede del rango
 * permitido, se informa al usuario. Si por el contrario, el numero es valido, 
 * guarda en la estructura de comunicacion con el main siempre y cuando no haya 
 * nada guardado en su lugar
 * 
 * Recibe:
 * value: puntero a string a analizar
 * parseInfo_t info: estructura para almacenar datos
 * 
 * Devuelve:
 * true (1) si el valor del string es valido
 * falso (0) si el string o su valor es invalido
 */
int rutLeftAngle (pChar value, parseInfo_t info)
{   
    if(info.modo != TRIANGULO) //si todavia no se selecciono modo, o ya se selecciono otro
    {
        fprintf(stderr, "Error, solo puede invocarse -leftAngle una vez \
                        establecido el modo en triangulo con '-type UNIFORME'\n");
        return false;
    }
    else if(info.triangulo.leftAngle > 0) //si ya se ingreso el angulo izquierdo
    {
        fprintf(stderr, "Error: -leftAngle invocado mas de una vez\n");
    }
    else  //si se esta trabajando en modo triangulo y todavia no se ingreso el angulo izquierdo
    {
        if(rutValidacion(value))    //si el string corresponde a un numero
        {   
            float angulo = atof(value);             //obtener angulo ingresado
            if((0.0 < angulo) && (angulo <= 90.0))  //si el numero esta dentro del rango permitido 
            {
                info.triangulo.leftAngle = angulo; //cargar el valor ingresado por el usuario
                return true;
            }
            else
            {
                fprintf(stderr, "Error: angulo %s* no se encuentra en el rango indicado (0* a 90*)", value);
                return false;
            }
        }
        else    //si el string no corresponde a un numero
        {    
            fprintf(stderr, "Error: angulo %s* con valor no numerico\n", value);
            return false;
        } 
    }
}

/* rutRightAngle
 * 
 * funcion que indica si el string recibido corresponde a un angulo valido (0 a 
 * 90 grados). Si el string no contiene un numero o el numero se excede del rango
 * permitido, se informa al usuario. Si por el contrario, el numero es valido, 
 * guarda en la estructura de comunicacion con el main siempre y cuando no haya 
 * nada guardado en su lugar
 * 
 * Recibe:
 * value: puntero a string a analizar
 * parseInfo_t info: estructura para almacenar datos
 * 
 * Devuelve:
 * true (1) si el valor del string es valido
 * falso (0) si el string o su valor es invalido
 */
int rutRightAngle (pChar value, parseInfo_t info)
{   
    if(info.modo != TRIANGULO) //si todavia no se selecciono modo, o ya se selecciono otro
    {
        fprintf(stderr, "Error, solo puede invocarse -rightAngle una vez \
                        establecido el modo en triangulo con '-type UNIFORME'\n");
        return false;
    }
    else if(info.triangulo.rightAngle > 0) //si ya se ingreso el angulo derecho
    {
        fprintf(stderr, "Error: -rightAngle invocado mas de una vez\n");
    }
    else  //si se esta trabajando en modo triangulo y todavia no se ingreso el angulo derecho
    {
        if(rutValidacion(value))    //si el string corresponde a un numero
        {   
            float angulo = atof(value);             //obtener angulo ingresado
            if((0.0 < angulo) && (angulo <= 90.0))  //si el numero esta dentro del rango permitido 
            {
                info.triangulo.rightAngle = angulo; //cargar el valor ingresado por el usuario
                return true;
            }
            else
            {
                fprintf(stderr, "Error: angulo %s* no se encuentra en el rango indicado (0* a 90*)", value);
                return false;
            }
        }
        else    //si el string no corresponde a un numero
        {    
            fprintf(stderr, "Error: angulo %s* con valor no numerico\n", value);
            return false;
        } 
    }
}

/* rutDatosNumericos
 * 
 * funcion que chequea si el string ingresado corresponde a un numero. En caso 
 * de no ser asi, lo indica al usuario
 *
 * Recibe:
 * value: puntero a string a analizar
 * parseInfo_t info: estructura para almacenar datos
 * 
 * Devuelve:
 * true (1) si el valor es valido
 * falso (0) si el valor es invalido
 */
int rutDatosNumericos (pChar value, parseInfo_t info)
{
    if(rutValidacion(value))    //si se ingresaron numeros, devolver true
        return true;
    else
        fprintf(stderr,"Error: caracteres invalidos en numero %s", value);
        return false;
}

/*rutError
 * 
 * funcion que indica al usuario que ha ingresado una key inexistente, le 
 * informa cuales son las keys que debe ingresar, y devuelve 0 indicando que 
 * hubo un error
 * 
 * Recibe: 
 * value: puntero a string
 * parseInfo_t info: estructura para almacenar datos
 * 
 * Devuelve:
 * false(0)
 */
int rutError (pChar value, parseInfo_t info)
{
    fprintf (stderr,"Error: key %s inexistente\n\
                     Valores posibles \n\
                     Para UNIFORME: -type, -lStart, -lEnd, -rightAngle, -leftAngle \n\
                     Para POLIGONO: -type, -lStart, -lEnd, -lConstant, -n \n\
                     Para MANDELBROT: -type, -x0, -xf, -y0, -yf \n", value);
    return false;
}

/* rutType
 * 
 * funcion que analiza si el valor de la key -Type es valido, es decir, si es 
 * UNIFORME, POLIGONO, O MANDELBROT, y lo guarda en la estructura de 
 * comunciacion con el main
 * 
 * Recibe:
 * value: puntero a string a analizar
 * parseInfo_t info: estructura para almacenar datos
 * 
 * Devuelve:
 * true (1) si el valor es valido
 * falso (0) si el valor es invalido o si ya se habia establecido un modo 
 * anteriormente
 */
int rutType (pChar value, parseInfo_t info)
{
    if(!(info.modo))   //si todavia no se selecciono un modo
    {
        if (strcmp(value, "UNIFORME") == 0) //strcmp devuelve 0 cuando los string son iguales
        {
            info.modo = TRIANGULO;  //indicar el modo seleccionado
            return true;
        }
        else if (strcmp(value, "POLIGONO") == 0)
        {
            info.modo = POLIGONO;  //indicar el modo seleccionado
            return true;
        }
        else if (strcmp(value, "MANDELBROT") == 0)
        {
            info.modo = MANDELBROT;  //indicar el modo seleccionado
            return true;
        }
        else
            fprintf(stderr,"Error: valor de -type %s es incorrecto.\n\
                            Valores posibles: UNIFORME, POLIGONO, O MANDELBROT\n", value);
            return false;
    }
    else
    {
        fprintf(stderr,"Error: -type invocado mas de una vez\n");
        return false;
    }
}

 /* rutValidacion
  * 
  * funcion que analiza que el string ingresado corresponda a un numero, tanto 
  * positivo como negativo. Admite numeros con decimales.
  * 
  * Recibe:
  * value: puntero al string a analizar
  * parseInfo_t info: estructura para almacenar datos
  * 
  * Devuelve
  * false(0) si no es un numero
  * true(1) si es numero
  */
int rutValidacion(pChar value)
{
     unsigned long i = strlen(value), j = 0;
    
    //analizar todos los caracteres desde 0 hasta i
    while (j< i)    
    {
        //caracteres aceptados: numeros en cualquier lugar y '-' solo en primera posicion
        if ( isdigit(value[j]) || ( value[0] == '-' && j == 0 ) )
            j++;
        else
            return false;
    }
    return true; 
}
