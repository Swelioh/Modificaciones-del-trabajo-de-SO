#ifndef KERNEL_MEMORY_UTILS_H_
#define KERNEL_MEMORY_UTILS_H_

#include <utils/utils.h>
#include "variablesGlobales.h"

// *********************************************
//              Prototipos
// *********************************************
void validar_argumentos (int cantidadArgumentos);
void inicializar_modulo(char* pathConfig);
void liberar_recursos(t_log* logger_kernel_memory,t_config* config_kernel_memory);

typedef enum {  //TODO: este enum deberia estar en utils global porque tiene que ser el mismo que use el scheduler y otros modulos.
    HANDSHAKE_CPU = 1,
    HANDSHAKE_KERNEL_SCHEDULER  = 3
    // OP_CREAR_PROCESO = 10,
} codigo_operacion;


#endif


