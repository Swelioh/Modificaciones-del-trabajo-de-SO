#ifndef SCHEDULER_GLOBALS_H_
#define SCHEDULER_GLOBALS_H_

#include <utils/utils.h>

// *********************************************
//              Variables globales
// *********************************************
//Config
extern t_config* config;
extern char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
extern char* PLANIFICATION_ALGORITHM; //algoritmo de planificación a utilizar
extern char ** QUEUES_ALGORITHMS; //Lista de algoritmos a utilizar en CMN
extern int RR_QUANTUM; //Tiempo de quantum para RR en mS
extern char* QUEUE_PREEMPTION; //Desalojo entre colas habilitado (string con "TRUE" O "FALSE")
extern int SUSPENSION_TIMEOUT; //Tiempo para suspender un proceso que está en IO en mS
extern char* IP_KERNEL_MEMORY; //Ip para conectarse al modulo kernel memory
extern char* PUERTO_KERNEL_MEMORY; //Idem para puerto
extern char* PUERTO_KERNEL_SCHEDULER; //Puerto que utiliza el scheduler como servidor

//Logger
extern t_log* logger;

#endif