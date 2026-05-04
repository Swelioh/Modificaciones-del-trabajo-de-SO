#ifndef SCHEDULER_COM_PROCESADOR_H_
#define SCHEDULER_COM_PROCESADOR_H_

#include<pthread.h> //Para crear hilos
#include "variablesGlobales.h"
#include <serializacion/estructuras.h> // Para conocer t_paquete y t_buffer
#include <serializacion/serializacion.h> // Para tener funciones de enviar y recibir paquetes
#include <variables_globales/variables_globales.h> // Para saber los codigos de operaciones
#include <serializacion/scheduler_cpu/serializacionesScheduler_Cpu.h> // Para saber serializar y deserializar paquetes de scheduler - cpu

// ************************************************************************
//                            ESTRUCTURAS
// ************************************************************************

// Estructura que tiene los argumentos para poder crear el hilo que maneja la conexion con la CPU. Tambien Van a ser las estrcuturas para administralos
/*typedef struct {
    int socket_cpu;
    t_ingreso_cpu info_cpu;
} t_handler_cpu;
*/

typedef struct {
    int socket_cpu;
    t_buffer* buffer;
} t_args_handler_cpu;

// ************************************************************************
//                            PROTOTIPOS
// ************************************************************************

// Funcion que administra la comunicacion con la CPU (Corre en un hilo)
void* handlerCPU(void* argumentos);

#endif