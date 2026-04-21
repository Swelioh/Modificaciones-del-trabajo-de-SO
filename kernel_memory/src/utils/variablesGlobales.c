#include "variablesGlobales.h"

// *********************************************
//              Variables globales
// *********************************************
//Config
t_config* config;
char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
int* SEGMENT_MAX_SIZE; //Tamaño máximo que puede tener cualquier segmento
char* ALLOCATION_STRATEGY; //Indica el algoritmo a utilizar para seleccionar un hueco libre, los valores pueden ser: BEST o WORST
int* INSTRUCTION_DELAY; //Tiempo en milisegundos que se debe esperar antes de contestar una petición de instrucción
int* COMPACTION_DELAY; //Tiempo en milisegundos que se debe esperar antes de dar por finalizada la compactación
char* SCRIPTS_BASEPATH; //Path donde se encuentran los archivos de pseudocódigo
char* PUERTO_KERNEL_MEMORY; //Idem para puerto

//Logger
t_log* logger;