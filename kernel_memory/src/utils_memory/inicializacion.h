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

typedef struct {
    int fd;
    int conexion_servidor;
} t_args;

#endif
