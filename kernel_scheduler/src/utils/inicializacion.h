#ifndef SCHEDULER_UTILS_H_
#define SCHEDULER_UTILS_H_

#include <utils/utils.h>
#include "variablesGlobales.h"

// *********************************************
//              Prototipos
// *********************************************
void validarArgumentos (int cantidadArgumentos);
void inicializarModulo(char* pathConfig);
void liberarModulo(t_log* logger,t_config* config);
#endif