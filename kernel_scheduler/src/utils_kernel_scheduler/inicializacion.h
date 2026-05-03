#ifndef SCHEDULER_INIT_H_
#define SCHEDULER_INIT_H_

#include <utils/utils.h> //Para usar funciones para crear servidores
#include<pthread.h>      //Para crear hilos
#include "variablesGlobales.h" // Para poder conocer variables globales en el config
#include <serializacion/estructuras.h> // Para que el main sepa que es un t_paquete
#include <serializacion/serializacion.h> // Para que el main sepa que es recibir_paquete_completo
#include <variables_globales/variables_globales.h> // Para tener los codigos de operacion
#include <serializacion/scheduler_io/serializacionesScheduler_Io.h> // Para saber serializar y deserializar paquetes

// *********************************************
//              PROTOTIPOS
// *********************************************
void validarArgumentos (int cantidadArgumentos);
void inicializarModulo(char* pathConfig);
int iniciarConexionKernelMemory(t_log* logger, char* ip_memory, char* puerto);
void liberarModulo();


void crear_proceso(char* path, int prioridad);
void procesar_fin(int pid);
#endif