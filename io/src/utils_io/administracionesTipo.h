#ifndef IO_ADMINISTRACIONES_H_
#define IO_ADMINISTRACIONES_H_

#include "variablesGlobales.h" //Para los loggers
#include <serializacion/estructuras.h>
#include <serializacion/serializacion.h>
#include <commons/string.h>
#include <serializacion/scheduler_io/serializacionesScheduler_Io.h> // Para saber serializar y deserializar paquetes

// *********************************************
//              PROTOTIPOS
// *********************************************
// Funciones generales
bool enviar_paquete_a_scheduler(int socket_scheduler, t_paquete* paquete);
bool mandarSolicitudAScheduler(int socket_scheduler);
t_paquete* recibir_paquete_de_scheduler (int socket_scheduler, uint32_t codigo_esperado);
// Administracion tipo sleep
void administrarIoTipoSleep(int socket_scheduler);
bool mandarRegresoTipoSleepAScheduler(int socket_scheduler, uint32_t pid);
// Administracion tipo stdin
void administrarIoTipoStdin(int socket_scheduler);
char* leerCadena(uint32_t tamanio);
bool reemplazarCaracterNewline(char* cadena, uint32_t tamanio);
void rellenarCadenaConBarraCero(char* cadena, uint32_t tamanio);
bool mandarRegresoTipoStdinAScheduler(int socket_scheduler, uint32_t pid, uint32_t largo, char* string);
// Administracion tipo stdout
void administrarIoTipoStdout(int socket_scheduler);
bool mandarRegresoTipoStdoutAScheduler(int socket_scheduler, uint32_t pid);
#endif 