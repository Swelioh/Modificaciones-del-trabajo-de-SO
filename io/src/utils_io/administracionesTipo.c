#include "administracionesTipo.h"
#include <unistd.h> //Para usar usleep

// Funcion que envia un paquete al scheduler
// Si no lo pudo enviar cierra el socket del scheduler y devuelve false, caso contrario devuelve true
bool enviar_paquete_a_scheduler(int socket_scheduler, t_paquete* paquete)
{
    // Envio el request al scheduler, si "enviar_paquete" me devuelve 0 o menos es que no pudo enviar la informacion (se desconecto el scheduler)
    bool exito = true;
    if(enviar_paquete(paquete, socket_scheduler) <= 0)
    {
        log_debug(logger, "Se desconecto el Kernel Scheduler, finalizando el programa...");
        close(socket_scheduler);
        exito = false;
    }
   	eliminar_paquete(paquete);
    return exito;
}

// Esta funcion manda al scheduler que el modulo esta listo para procesar una IO
bool mandarSolicitudAScheduler(int socket_scheduler)
{
    // Armo paquete y envio
    t_paquete* paquete = armar_paquete(A_LA_ESPERA_IO, NULL);
    // Envio paquete
    return enviar_paquete_a_scheduler(socket_scheduler, paquete);
}

// Funcion que recibe un paquete del scheduler y verifica que sea el codigo de operacion esperado
// En caso de error o que no coincida el codigo esperado con el que se recibio cierra el socket de scheduler y devuelve NULL. Caso contrario devuelve el puntero al paquete
t_paquete* recibir_paquete_de_scheduler (int socket_scheduler, uint32_t codigo_esperado)
{
    // Recibo paquete
    t_paquete* paquete_regreso = recibir_paquete_completo(socket_scheduler);
    // Si me devolvio null casi seguro porque se forzo el cierre del modulo
    if(paquete_regreso == NULL){
        log_debug(logger, "Se desconecto el Kernel Scheduler, finalizando el programa...");
        close(socket_scheduler);
        return NULL;
    }
    // Si el codigo de operacion es otro al que esperaba
    if(paquete_regreso->codigo_operacion != codigo_esperado){
        log_debug(logger, "No se esperaba ese codigo de operacion.");
        eliminar_paquete(paquete_regreso);
        close(socket_scheduler);
        return NULL;
    }
    return paquete_regreso;
}


// Funcion que administra la comunicacion con el scheduler cuando el modulo de IO es tipo SLEEP
void administrarIoTipoSleep(int socket_scheduler)
{
    while(1){
        // Le mandamos al scheduler un mensaje para decirle que estamos listos para procesar, si el scheduler se desconecto nos vamos de esta funcion
        if(mandarSolicitudAScheduler(socket_scheduler) == false)
            return;

        // Nos quedamos bloqueados hasta que el scheduler nos mande una tarea de tipo SLEEP, si el scheduler se desconecto nos vamos de esta funcion
        t_paquete* paquete = recibir_paquete_de_scheduler(socket_scheduler, SOLICITUD_IO_SLEEP);
        if(paquete == NULL)
            return;

        // Deserializo la solicitud para asi obtener el pid del proceso (para asi mandarlo despues en el regreso)
        t_solicitud_io_sleep solicitud;
        deserializarSolicitudIoSleep(paquete->buffer, &solicitud);

        // Ejecuto lo propio de la IO de este tipo
        log_info(logger, "## PID: %d - Inicio de IO", solicitud.pid_proceso); //LOG_OBLIGATORIO
        log_info(logger, "## PID: %d - Haciendo sleep por %d milisegundos.", solicitud.pid_proceso, solicitud.tiempo_sleep_ms); //LOG_OBLIGATORIO
        usleep(solicitud.tiempo_sleep_ms * 1000); // Usamos la funcion que duerme en microsegundos, como nosotros teniamos milisegundos multiplicamos por 1000
        log_info(logger, "## PID: %d - Fin de IO”", solicitud.pid_proceso); //LOG_OBLIGATORIO

        // Le mando al scheduler que termine, si el scheduler se desconecto nos vamos de esta funcion
        if(mandarRegresoTipoSleepAScheduler(socket_scheduler, solicitud.pid_proceso) == false)
        {
            eliminar_paquete(paquete);
            return;
        }

        // Libero la memoria del paquete
        eliminar_paquete(paquete);
    }
}
// Esta funcion manda al scheduler que se termino de procesar la solicitud de tipo sleep
bool mandarRegresoTipoSleepAScheduler(int socket_scheduler, uint32_t pid)
{
    // Armo la request
    t_regreso_io_sleep regreso;
    regreso.pid_proceso = pid;
    t_paquete* paquete = armar_paquete(REGRESO_IO_SLEEP, &regreso);
    // La envio
    return enviar_paquete_a_scheduler(socket_scheduler, paquete);
}

// Funcion que administra la comunicacion con el scheduler cuando el modulo de IO es tipo STDIN
void administrarIoTipoStdin(int socket_scheduler)
{
    while(1){
        // Le mandamos al scheduler un mensaje para decirle que estamos listos para procesar, si el scheduler se desconecto nos vamos de esta funcion
        if(mandarSolicitudAScheduler(socket_scheduler) == false)
            return;

        // Nos quedamos bloqueados hasta que el scheduler nos mande una tarea de tipo SLEEP, si el scheduler se desconecto nos vamos de esta funcion
        t_paquete* paquete = recibir_paquete_de_scheduler(socket_scheduler, SOLICITUD_IO_STDIN);
        if(paquete == NULL)
            return;

        // Deserializo la solicitud para asi obtener el pid del proceso y el tamanio maximo a leer
        t_solicitud_io_stdin solicitud;
        deserializarSolicitudIoStdin(paquete->buffer, &solicitud);

        // Ejecuto lo propio de la IO de este tipo
        log_info(logger, "## PID: %d - Inicio de IO", solicitud.pid_proceso); //LOG_OBLIGATORIO
        log_info(logger, "## PID: %d - Ingrese %d caracteres:", solicitud.pid_proceso, solicitud.tamanioALeer); //LOG_OBLIGATORIO

        // Leo la cadena que el usuario ingresa por consola
        char* cadena = leerCadena(solicitud.tamanioALeer);
        if(cadena == NULL) // Si fallo el fgets de la funcion nos vamos
        {
            eliminar_paquete(paquete);
            return;
        }

        log_info(logger, "## PID: %d - Fin de IO", solicitud.pid_proceso); //LOG_OBLIGATORIO

        // Le mando al scheduler que termine, si el scheduler se desconecto nos vamos de esta funcion
        if(mandarRegresoTipoStdinAScheduler(socket_scheduler, solicitud.pid_proceso, solicitud.tamanioALeer, cadena) == false)
        {
            free(cadena);
            eliminar_paquete(paquete);
            return;
        }

        eliminar_paquete(paquete);
    }
}

// Funcion que busca el caracter \n en la cadena y lo reemplaza por el \0
// Devuelve un bool diciendo si la cadena tenia o no el \n
bool reemplazarCaracterNewline(char* cadena, uint32_t tamanio)
{
    bool tiene_enter = false;
    for (int i = 0; i < tamanio; i++) {
        if (cadena[i] == '\n') {
            cadena[i] = '\0';
            tiene_enter = true;
            break;
        }
    }
    return tiene_enter;
}
// Si la cadena ingresada es menor al tamanio maximo completo con \0
void rellenarCadenaConBarraCero(char* cadena, uint32_t tamanio)
{
    int largo = string_length(cadena);
    for (int i = largo; i < tamanio; i++) {
        cadena[i] = '\0';
    }
}
// Se encarga de leer una cadena por consola del tamanio especificado en el argumento
char* leerCadena(uint32_t tamanio)
{
    // Pido memoria para almacenar el string
    char* cadena = malloc(sizeof(char)*tamanio);

    // Leo el string con el tamanio maximo -> El fgets me lo corta a la cantidad que yo le pedi leer
    if (fgets(cadena, tamanio, stdin) == NULL) {
        if(tamanio == 0)
            log_debug(logger, "El largo de la cadena a leer era 0. Cerrando el modulo...");
        else
            log_debug(logger, "Hubo un error al leer la cadena del usuario. Cerrando el modulo...");
        free(cadena);
        return NULL;
    }

    // Me fijo si aparecio el \n en lo que lei (esto lo hacemos para limpiar el buffer por si lo que escribio el usuario excedio el maximo que leiamos)
    bool tieneEnter = reemplazarCaracterNewline(cadena, tamanio);
    
    // Si mi cadena no tenia el enter es porque lo que escribio el usuario excedio lo que podia leer fgets, por lo tanto sigue habiendo cosas en el buffer, hay que limpiarlo
    if (!tieneEnter) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF); //Hago un while 1 hasta que encuentro el \n o el EOF
    }

    // Si la cadena es menor al tamanio indicado relleno con \0
    rellenarCadenaConBarraCero(cadena, tamanio);

    return cadena;
}
// Esta funcion manda al scheduler que se termino de procesar la solicitud de tipo stdin
bool mandarRegresoTipoStdinAScheduler(int socket_scheduler, uint32_t pid, uint32_t largo, char* string)
{
    // Armo la request
    t_regreso_io_stdin regreso;
    regreso.pid_proceso = pid;
    regreso.largo_cadena = largo-1; // PORQUE CUANDO ARMO EL PAQUETE LA FUNCION DE AGREGAR_STRING LE SUMA UNO PARA EL \0 QUE YO YA ESTOY CONSIDERANDO ACA
    regreso.cadena = string;
    t_paquete* paquete = armar_paquete(REGRESO_IO_STDIN, &regreso);
    // La envio
    return enviar_paquete_a_scheduler(socket_scheduler, paquete);
}

void administrarIoTipoStdout(int socket_scheduler)
{
    while(1){
        mandarSolicitudAScheduler(socket_scheduler);
    }
}