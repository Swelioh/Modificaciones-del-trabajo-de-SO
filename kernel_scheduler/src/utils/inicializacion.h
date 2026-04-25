#ifndef SCHEDULER_UTILS_H_
#define SCHEDULER_UTILS_H_

#include <utils/utils.h>
#include "variablesGlobales.h"
#include<pthread.h>
// en utils.h o en un header propio (ej: protocolo.h)
typedef enum {

    // 🔌 Handshake
    HANDSHAKE_CPU = 1,
    HANDSHAKE_IO  = 2,

    // 🧠 CPU → Kernel
    OP_CREAR_PROCESO = 10
    OP_FIN_PROCESO = 11,
    OP_SYSCALL     = 11,
    OP_INTERRUPCION= 12,

    // 💾 IO → Kernel
    OP_IO_FIN      = 20,

    // 🧠 Kernel → CPU
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