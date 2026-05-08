#ifndef IO_GLOBALS_H_
#define IO_GLOBALS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <unistd.h>
#include <serializacion/scheduler_io/serializacionesScheduler_Io.h> // Para conocer el enum de tipos de IO

// **************************************************
//            Variables globales generales
// **************************************************
//Config
extern t_config* config;
extern char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
extern char* IP_KERNEL_SCHEDULER; //Ip para conectarse al modulo kernel scheduler
extern char* PUERTO_KERNEL_SCHEDULER; //Puerto que utiliza el scheduler como servidor

//Logger
extern t_log* logger;

extern t_tipos_io tipo_modulo; //Guardo que tipo de modulo es

#endif