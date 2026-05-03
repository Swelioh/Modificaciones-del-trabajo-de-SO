#ifndef IO_INIT_H_
#define IO_INIT_H_

#include <utils/utils.h> //Para usar funciones para conectarme a un servidor
#include "variablesGlobales.h"
#include <serializacion/estructuras.h>
#include <serializacion/serializacion.h>
#include <variables_globales/variables_globales.h> // Para saber codigos de operacion
#include <serializacion/scheduler_io/serializacionesScheduler_Io.h> // Para conocer el enum de tipos de IO

// *********************************************
//              PROTOTIPOS
// *********************************************
void validarArgumentos (int cantidadArgumentos);
void inicializarModulo(char* pathConfig, char* tipo);
int iniciarConexionKernelScheduler(t_log* logger);
void enviarIngresoAScheduler(int socket_scheduler);
void liberarModulo(t_log* logger_a_destruir,t_config* config_a_destruir);

t_tipos_io tipo_io_string_a_enum(char* tipo);

#endif