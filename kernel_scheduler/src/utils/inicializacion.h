#ifndef SCHEDULER_UTILS_H_
#define SCHEDULER_UTILS_H_

#include <utils/utils.h>
#include "variablesGlobales.h"
#include<pthread.h>

// en utils.h o en un header propio (ej: protocolo.h)
typedef enum {
    // operaciones entre módulos
    OP_CREAR_PROCESO = 0,
    OP_SYSCALL       = 1,
    OP_FIN_PROCESO   = 2,
    OP_FIN_QUANTUM   = 3,
    OP_INTERRUPCION  = 4,

    // handshake — quién se conecta
    TIPO_CPU         = 10,
    TIPO_IO          = 11,

    // syscalls
    SYSCALL_SLEEP        = 20,
    SYSCALL_STDIN        = 21,
    SYSCALL_STDOUT       = 22,
    SYSCALL_MUTEX_CREATE = 23,
    SYSCALL_MUTEX_LOCK   = 24,
    SYSCALL_MUTEX_UNLOCK = 25,
    SYSCALL_MEM_ALLOC    = 26,
    SYSCALL_MEM_FREE     = 27,
    SYSCALL_EXIT         = 28,
} t_cod_op;

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