#ifndef VARIABLESGLOBALES_H_
#define VARIABLESGLOBALES_H_

#include <commons/log.h>
#include <commons/config.h>
#include <utils/utils.h>
// **************************************************
//              Variables globales generales
// **************************************************
// Config
extern t_config *config;
extern char *LOG_LEVEL;               // Nivel de detalle máximo a mostrar.
extern t_log *logger;
extern char *PUERTO_KERNEL_SCHEDULER; 
extern char *PUERTO_KERNEL_MEMORY; 
extern char *PUERTO_MEMORY_STICK; 
extern char *IP_KERNEL_SCHEDULER; 
extern char *IP_KERNEL_MEMORY; 
extern char *IP_MEMORY_STICK; 
extern uint32_t IDENTIFICADOR;




#endif