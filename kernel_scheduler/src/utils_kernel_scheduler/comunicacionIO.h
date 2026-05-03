#ifndef SCHEDULER_COM_IO_H_
#define SCHEDULER_COM_IO_H_

#include<pthread.h> //Para crear hilos
#include "variablesGlobales.h"
#include <serializacion/estructuras.h> // Para saber que es un t_paquete y t_buffer
#include <serializacion/serializacion.h> // Para tener las funciones de enviar paquete y recibir paquete completo
#include <variables_globales/variables_globales.h>  // Para saber los codigos de operaciones
#include <serializacion/scheduler_io/serializacionesScheduler_Io.h> // Para saber serializar y deserializar paquetes

typedef struct {
    int socket_io;
    t_ingreso_io info_io;
} t_handler_io; // Estructura que tiene los argumentos para poder crear el hilo que maneja la conexion con la IO

// ************************************************************************
//                            PROTOTIPOS
// ************************************************************************
void iniciarIO(int socket_io, t_buffer* buffer);
char* tipo_io_enum_a_string(uint8_t tipo);
bool procesoInicialComunicacion(int socketIo, char* tipo);
bool enviar_paquete_a_io(int socketIo, char* tipo, t_paquete* paquete_solicitud);
t_paquete* recibir_paquete_de_io (int socket_io, char* tipo, uint32_t codigo_esperado);
// Tipo sleep
void* handlerSleepIO(void* argumentos);
bool enviarSolicitudTipoSleep(int socket_io, uint32_t pid, uint32_t tiempomS);
bool recibirRespuestaTipoSleep(int socketIo, uint32_t* pid_proceso);
// Tipo stdin
void* handlerStdinIO(void* argumentos);
bool enviarSolicitudTipoStdin(int socket_io, uint32_t pid, uint32_t tamanio);
bool recibirRespuestaTipoStdin(int socketIo, t_regreso_io_stdin* mensaje_regreso);
// Tipo stdout
void* handlerStoudIO(void* argumentos);

#endif