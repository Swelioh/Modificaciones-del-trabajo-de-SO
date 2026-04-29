#include "variablesGlobales.h"

// *********************************************
//              Variables globales
// *********************************************
//Config
t_config* config;
char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
char* IP_KERNEL_SCHEDULER; //Ip para conectarse al modulo kernel scheduler
char* PUERTO_KERNEL_SCHEDULER; //Puerto que utiliza el scheduler como servidor

//Logger
t_log* logger;

t_tipos_io tipo_modulo; //Guardo que tipo de modulo es