#ifndef KERNEL_MEMORY_GLOBALS_H_
#define KERNEL_MEMORY_GLOBALS_H_

#include <utils/utils.h>

// *********************************************
//              Variables globales
// *********************************************
//Config
extern t_config* config;
extern char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
extern int SEGMENT_MAX_SIZE; //Tamaño máximo que puede tener cualquier segmento
extern char* ALLOCATION_STRATEGY; //Indica el algoritmo a utilizar para seleccionar un hueco libre, los valores pueden ser: BEST o WORST
extern int INSTRUCTION_DELAY; //Tiempo en milisegundos que se debe esperar antes de contestar una petición de instrucción
extern int COMPACTION_DELAY; //Tiempo en milisegundos que se debe esperar antes de dar por finalizada la compactación
extern char* SCRIPTS_BASEPATH; //Path donde se encuentran los archivos de pseudocódigo
extern char* PUERTO_KERNEL_MEMORY; //Idem para puerto

//Logger
extern t_log* logger;

#endif