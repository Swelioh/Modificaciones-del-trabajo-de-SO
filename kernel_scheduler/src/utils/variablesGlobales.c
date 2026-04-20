#include "variablesGlobales.h"

// *********************************************
//              Variables globales
// *********************************************
//Config
t_config* config;
char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
char* PLANIFICATION_ALGORITHM; //algoritmo de planificación a utilizar
char ** QUEUES_ALGORITHMS; //Lista de algoritmos a utilizar en CMN
int RR_QUANTUM; //Tiempo de quantum para RR en mS
char* QUEUE_PREEMPTION; //Desalojo entre colas habilitado (string con "TRUE" O "FALSE")
int SUSPENSION_TIMEOUT; //Tiempo para suspender un proceso que está en IO en mS
char* IP_KERNEL_MEMORY; //Ip para conectarse al modulo kernel memory
char* PUERTO_KERNEL_MEMORY; //Idem para puerto
char* PUERTO_KERNEL_SCHEDULER; //Puerto que utiliza el scheduler como servidor

//Logger
t_log* logger;