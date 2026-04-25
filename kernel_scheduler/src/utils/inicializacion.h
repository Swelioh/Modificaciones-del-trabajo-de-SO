#ifndef SCHEDULER_UTILS_H_
#define SCHEDULER_UTILS_H_

#include <utils/utils.h> //Para usar funciones para crear servidores
#include<pthread.h> //Para crear hilos
#include "variablesGlobales.h"
#include <serializacion/estructuras.h>
#include <serializacion/serializacion.h>


// en utils.h o en un header propio (ej: protocolo.h)S
typedef enum {
    HANDSHAKE_CPU = 1,
    HANDSHAKE_IO  = 2,

    OP_CREAR_PROCESO = 10,
    OP_FIN_PROCESO = 11,
    OP_SYSCALL     = 11,
    OP_INTERRUPCION= 12,

    OP_IO_FIN      = 20,

    OP_EJECUTAR_PROCESO = 30

} codigo_operacion;

// *********************************************
//              Prototipos
// *********************************************
void validarArgumentos (int cantidadArgumentos);
void inicializarModulo(char* pathConfig);
void liberarModulo(t_log* logger_a_destruir,t_config* config_a_destruir);
void* hilo_aceptador(void* arg);
void* hilo_handler(void* arg);
void crear_proceso(char* path, int prioridad);
void procesar_fin(int pid);
#endif