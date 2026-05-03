#include "comunicacionIO.h"

// Funcion que se encarga de crear el hilo para atender al modulo de io en base al tipo del mismo
void iniciarIO(int socket_io, t_buffer* buffer)
{
    // Creo la estructura donde deserializo el mensaje
    t_ingreso_io mensaje_ingreso;
    deserializarIngresoIO(buffer, &mensaje_ingreso);

    // Creo la estructura que va a almacenar los argumentos que se pasan a la funcion del p_thread
    t_handler_io* args = malloc(sizeof(t_handler_io));
    args->socket_io = socket_io;
    args->info_io = mensaje_ingreso;

    pthread_t hilo;
    // Dependiendo el tipo de IO inicializo un modulo para manejar esa conexion
    switch(mensaje_ingreso.tipo_io)
    {
        case SLEEP:
            pthread_create(&hilo, NULL, handlerSleepIO, args);
            pthread_detach(hilo);
            break;
        case STDIN:
            pthread_create(&hilo, NULL, handlerStdinIO, args);
            pthread_detach(hilo);
            break;
        case STDOUT:
            pthread_create(&hilo, NULL, handlerStoudIO, args);
            pthread_detach(hilo);
            break;
        default:
            log_debug(logger, "No se reconoce ese tipo de modulo IO. Tipo recibido %d", mensaje_ingreso.tipo_io);
            free(args);
    }
    return;
}

// Funcion que me convierte el enum del tipo de IO a un string
char* tipo_io_enum_a_string(uint8_t tipo)
{
    switch(tipo)
    {
        case SLEEP:
            return "SLEEP";
        case STDIN:
            return "STDIN";
        case STDOUT:
            return "STDOUT";
        default:
            return "IO_INVALIDO";
    }
}

// Este es el proceso de comunicacion comun a todos los hilos que manejen IOs, esperamos a que el IO nos mande el paquete que avisa que esta listo para procesar una IO
// Devuelve True si salio bien y false si ocurrio un error
bool procesoInicialComunicacion(int socketIo, char* tipo)
{
    // Recibimos la solicitud del modulo de IO para saber que esta disponible para procesar una interrupcion
    t_paquete* paquete_io_listo = recibir_paquete_completo(socketIo);

    // Si paquete es NULL es porque algun recv de "recibir_paquete_completo" dio -1, lo mas probable es porque se cerro de forma forzada el modulo de IO. 
    // No hace falta eliminar el paquete porque ya lo hizo "recibir_paquete_completo" cuando fallo
    if(paquete_io_listo == NULL){
        log_debug(logger, "Se desconecto un modulo de IO tipo %s", tipo);
        close(socketIo);
        return false;
    }
    // Si el paquete que me llego fue distinto al que esperaba
    if(paquete_io_listo->codigo_operacion != A_LA_ESPERA_IO)
    {
        log_debug(logger, "Hubo un error en la comunicacion con el modulo de IO tipo %s", tipo);
        close(socketIo);
        eliminar_paquete(paquete_io_listo);
        // TODO: Si el codigo de operacion que se recibio en el proceso inicial no era el esperado, que hacemos?
        return false;
    }

    eliminar_paquete(paquete_io_listo);
    return true;
}
// Funcion que envia paquete a modulo IO
// En caso de error cierra el socket y devuelve false. Caso contrario devuelve true.
bool enviar_paquete_a_io(int socketIo, char* tipo, t_paquete* paquete_solicitud)
{
    // Envio el request al modulo de IO, si enviar_paquete me devuelve 0 o menos es que no pudo enviar la informacion (se desconecto el cliente)
    bool exito = true;
    if(enviar_paquete(paquete_solicitud, socketIo) <= 0)
    {
        log_debug(logger, "Se desconecto un modulo de IO tipo %s", tipo);
        close(socketIo);
        exito = false;
    }

    eliminar_paquete(paquete_solicitud);
    return exito;
}
// Funcion que recibe un paquete y verifica que sea el codigo de operacion esperado
// En caso de error o que no coincida el codigo esperado con el que se recibio cierra el socket de IO y devuelve NULL. Caso contrario devuelve el puntero al paquete
t_paquete* recibir_paquete_de_io (int socket_io, char* tipo, uint32_t codigo_esperado)
{
    // Recibo paquete
    t_paquete* paquete_regreso = recibir_paquete_completo(socket_io);
    // Si me devolvio null casi seguro porque se forzo el cierre del modulo
    if(paquete_regreso == NULL){
        log_debug(logger, "Se desconecto un modulo de IO tipo %s", tipo);
        close(socket_io);
        return NULL;
    }
    // 
    if(paquete_regreso->codigo_operacion != codigo_esperado){
        log_debug(logger, "Hilo de comunicacion con IO tipo '%s' -> No se esperaba ese codigo de operacion.", tipo);
        eliminar_paquete(paquete_regreso);
        close(socket_io);
        return NULL;
    }

    return paquete_regreso;
}

// Funcion que administra la comunicacion con los IO tipo SLEEP
void* handlerSleepIO(void* argumentos){
    // Casteo el void* a la estructura que guarda los argumentos
    t_handler_io* args = (t_handler_io*) argumentos;
    // Me quedo con los datos que necesito y libero la memoria que habia pedido para almacenar los argumentos
    int socketIo = args->socket_io;
    t_ingreso_io info_io = args->info_io;
    free(args);

    log_debug(logger, "Se conecto un modulo de IO tipo SLEEP");

    while(1){
        // La IO me avisa que esta disponible para procesar una nueva solicitud
        if(procesoInicialComunicacion(socketIo, tipo_io_enum_a_string(info_io.tipo_io)) == false)
            return NULL;
    
        // El modulo IO esta disponible para procesar, ahora espero a que haya alguna tarea de tipo sleep pendiente por realizar
        sem_wait(&(semaforo_tareas_io_pendientes[SLEEP]));

        // Cuando hay una tarea para este modulo, la saco de la lista para que los demas hilos de este tipo de IO no la vean mas (Hay que liberar la memoria de esa tarea despues)
        pthread_mutex_lock(&mutex_lista_tareas_io[SLEEP]);
        t_tarea_io* tarea = queue_pop(tareas_io_pendientes[SLEEP]);
        pthread_mutex_unlock(&mutex_lista_tareas_io[SLEEP]);

        // Creo la request que le voy a mandar al io en base a la tarea que saque, si hubo un error en el envio seguro fue porque se forzo el cierre del modulo
        if(enviarSolicitudTipoSleep(socketIo, tarea->pid_proceso_relacionado, tarea->tiempoSleepEnMs) == false){
            //TODO: Ver que pasa si no se la pudo mandar la solicitud de IO (Yo creo que deberiamos volver a ponerla en la lista)
            return NULL;
        }

        // Me bloqueo hasta que el modulo de IO me devuelva que termino de procesar
        uint32_t pid_proceso;
        if(recibirRespuestaTipoSleep(socketIo, &pid_proceso) == false)
        {
            //TODO: Si el modulo de IO se desconecto/cerro cuando estaba realizando la tarea, que hacemos?
            return NULL;
        }

        log_info(logger, "## (%d) finalizó IO y pasa a READY / SUSP. READY", pid_proceso); //LOG_OBLIGATORIO

        //TODO: Poner el proceso que estaba en bloqueado en READY o SUSP_READY dependiendo el caso

        // Una vez que termine de procesar esta interrupcion puedo liberar la memoria de la tarea de la cola
        free(tarea);
    }
    
    return NULL;
}
// Funcion que le envia al modulo de IO la solicitud para que realice una operacion tipo SLEEP
// Devuelve true si la pudo mandar o false si hubo un error (en ese caso cierra el socket del IO)
bool enviarSolicitudTipoSleep(int socket_io, uint32_t pid, uint32_t tiempomS)
{
    // Armo el request
    t_solicitud_io_sleep solicitud;
    solicitud.pid_proceso = pid;
    solicitud.tiempo_sleep_ms = tiempomS;
    t_paquete* paquete_solicitud = armar_paquete_scheduler_io(SOLICITUD_IO_SLEEP, &solicitud);
    // La envio
    return enviar_paquete_a_io(socket_io, "SLEEP", paquete_solicitud);
}
// Funcion que espera el mensaje de la IO diciendo que termino la operacion tipo SLEEP
// Devuelve false si ocurrio un error (ademas de cerrar el socket de IO). Caso contrario devuelve true y carga en el puntero al uint32 el pid del proceso
bool recibirRespuestaTipoSleep(int socketIo, uint32_t* pid_proceso)
{
    t_paquete* paquete_regreso = recibir_paquete_de_io(socketIo, "SLEEP", REGRESO_IO_SLEEP);
     // Si me devolvio null es porque hubo error, ya "recibir_paquete_io" me cerro el socket, por lo que solo tengo que devolver false
    if(paquete_regreso == NULL){
        return false;
    }
    // Deserializo el mensaje y devuelvo el pid para que asi lo podamos mandar a READY o SUSP READY
    t_regreso_io_sleep mensaje_regreso;
    deserializarRegresoIOSleep(paquete_regreso->buffer, &mensaje_regreso);
    eliminar_paquete(paquete_regreso);
    *pid_proceso = mensaje_regreso.pid_proceso;
    return true;
}


// Funcion que administra la comunicacion con los IO tipo STDIN
void* handlerStdinIO(void* argumentos){
    // Casteo el void* a la estructura que guarda los argumentos
    t_handler_io* args = (t_handler_io*) argumentos;
    // Me quedo con los datos que necesito y libero la memoria que habia pedido para almacenar los argumentos
    int socketIo = args->socket_io;
    t_ingreso_io info_io = args->info_io;
    free(args);

    log_debug(logger, "Se conecto un modulo de IO tipo STDIN");

    while(1){
        // La IO me avisa que esta disponible para procesar una nueva solicitud
        if(procesoInicialComunicacion(socketIo, tipo_io_enum_a_string(info_io.tipo_io)) == false)
            return NULL;
    
        // El modulo IO esta disponible para procesar, ahora espero a que haya alguna tarea de tipo STDIN pendiente por realizar
        sem_wait(&(semaforo_tareas_io_pendientes[STDIN]));

        // Cuando hay una tarea para este modulo, la saco de la lista para que los demas hilos de este tipo de IO no la vean mas
        pthread_mutex_lock(&mutex_lista_tareas_io[STDIN]);
        t_tarea_io* tarea = queue_pop(tareas_io_pendientes[STDIN]);
        pthread_mutex_unlock(&mutex_lista_tareas_io[STDIN]);

        // Creo la request que le voy a mandar al io en base a la tarea que saque, si hubo un error en el envio seguro fue porque se forzo el cierre del modulo
        if(enviarSolicitudTipoStdin(socketIo, tarea->pid_proceso_relacionado, tarea->tamanioALeerEscribir) == false){
            //TODO: Ver que pasa si no se la pudo mandar la solicitud de IO (Yo creo que deberiamos volver a ponerla en la lista)
            return NULL;
        }

        // Me bloqueo hasta que el modulo de IO me devuelva que termino de procesar
        t_regreso_io_stdin mensaje_regreso;
        if(recibirRespuestaTipoStdin(socketIo, &mensaje_regreso) == false)
        {
            //TODO: Si el modulo de IO se desconecto/cerro cuando estaba realizando la tarea, que hacemos?
            return NULL;
        }

        log_debug(logger, "Se recibio la cadena '%s' de una IO de STDIN para el proceso de PID %d.", mensaje_regreso.cadena, mensaje_regreso.pid_proceso);

        log_info(logger, "## (%d) finalizó IO y pasa a READY / SUSP. READY", mensaje_regreso.pid_proceso); //LOG_OBLIGATORIO

        //TODO: Mandar cadena y direccion logica (esta en la estructura de la tarea_io) a Kernel Memory para que la guarde.
        //TODO: Poner el proceso que estaba en bloqueado en READY o SUSP_READY dependiendo el caso

        // Una vez que termine de procesar esta interrupcion puedo liberar la memoria de la tarea de la cola
        free(mensaje_regreso.cadena);
        free(tarea);
    }
    
    return NULL;
}

// Funcion que le envia al modulo de IO la solicitud para que realice una operacion tipo STDIN
// Devuelve true si la pudo mandar o false si hubo un error (en ese caso cierra el socket del IO)
bool enviarSolicitudTipoStdin(int socket_io, uint32_t pid, uint32_t tamanio)
{
    // Armo el request
    t_solicitud_io_stdin solicitud;
    solicitud.pid_proceso = pid;
    solicitud.tamanioALeer = tamanio;
    t_paquete* paquete_solicitud = armar_paquete_scheduler_io(SOLICITUD_IO_STDIN, &solicitud);
    // Envio la solicitud
    return enviar_paquete_a_io(socket_io, "STDIN", paquete_solicitud);
}
// Funcion que espera el mensaje de la IO diciendo que termino la operacion tipo STDIN
// En caso de error cierra el socket y devuelve false. Caso contrario devuelve true con los datos cargados en el puntero de mensaje_regreso
bool recibirRespuestaTipoStdin(int socketIo, t_regreso_io_stdin* mensaje_regreso)
{
    t_paquete* paquete_regreso = recibir_paquete_de_io(socketIo, "STDIN", REGRESO_IO_STDIN);
     // Si me devolvio null es porque hubo un error, ya "recibir_paquete_io" me cerro el socket por lo que solo devuelvo false
    if(paquete_regreso == NULL){
        return false;
    }
    // Deserializo el mensaje y devuelvo la estructura con el mensaje y el pid del proceso
    deserializarRegresoIOStdin(paquete_regreso->buffer, mensaje_regreso);
    eliminar_paquete(paquete_regreso);
    return true;
}



// TODO: Hacer stdout
void* handlerStoudIO(void* argumentos){
    free(argumentos);
    log_debug(logger, "Se conecto un modulo de IO tipo STDOUT");
    return NULL;
}