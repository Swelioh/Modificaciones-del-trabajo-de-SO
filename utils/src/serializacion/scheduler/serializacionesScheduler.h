#ifndef SCHEDULER_SERIALIZACION_H_
#define SCHEDULER_SERIALIZACION_H_

#include <unistd.h>
#include <serializacion/estructuras.h> // La necesito para poder saber que es un paquete o un buffer
#include <serializacion/serializacion.h> // La necesito porque tiene las funciones de crear paquete, o de agregar variables al paquete

// ********************************************************************************
//                          ESTRUCTURAS DE MENSAJES
// *********************************************************************************

typedef struct
{
	uint32_t identificador_cpu;
} t_ingreso_cpu; //Modeliza el mensaje que envia CPU al SCHEDULER avisando que se dispone de una CPU nueva para el procesamiento de datos

typedef struct {;   
    uint8_t  tipo_io;  
} t_ingreso_io; //Modeliza el mensaje que envia IO al SCHEDULER avisando que tipo de modulo fue el que se conecto

typedef struct {;   
	uint32_t pid_proceso;
    uint32_t  tiempo_sleep_ms;  
} t_solicitud_io_sleep; //Modeliza el mensaje que envia SCHEDULER a IO indicando el tiempo de sleep en mS

typedef struct {;   
	uint32_t pid_proceso;
} t_regreso_io_sleep; //Modeliza el mensaje que envia IO a SCHEDULER indicando que se termino la operacion de SLEEP


typedef struct {;   
	uint32_t pid_proceso;
    uint32_t tamanioALeer;  
} t_solicitud_io_stdin; //Modeliza el mensaje que envia SCHEDULER a IO indicando el tiempo de sleep en mS
typedef struct {;   
	uint32_t pid_proceso;
    int32_t  largo_cadena;
	char* cadena;
} t_regreso_io_stdin; //Modeliza el mensaje que envia IO a SCHEDULER indicando que se termino la operacion de STDIN
typedef struct {
    uint32_t pid_proceso;
    uint32_t largo_cadena;
    char* cadena;
} t_solicitud_io_stdout;
typedef struct {;   
	uint32_t pid_proceso;
} t_regreso_io_stdout; //Modeliza el mensaje que envia IO a SCHEDULER indicando que se termino la operacion de STDOUT

// ********************************************************************************
//                             ESTRUCTURAS GLOBALES
// *********************************************************************************

typedef enum
{
	IO_INVALIDO = -1,
	SLEEP = 0,
	STDIN,
	STDOUT,
	CANTIDAD_TOTAL_IO
} t_tipos_io; // Enum con los diferentes tipos de modulos IO que puedo tener

// ********************************************************************************
//                          PROTOTIPOS DE SERIALIZACON
// ********************************************************************************

void serializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu struct_a_serializar);
void deserializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu* struct_donde_deserializo);
void serializarIngresoIO(t_buffer *buffer, t_ingreso_io struct_a_serializar);
void deserializarIngresoIO(t_buffer *buffer, t_ingreso_io* struct_donde_deserializo);

void serializarSolicitudIoSleep(t_buffer *buffer, t_solicitud_io_sleep struct_a_serializar);
void deserializarSolicitudIoSleep(t_buffer *buffer, t_solicitud_io_sleep* struct_donde_deserializo);
void serializarRegresoIOSleep(t_buffer *buffer, t_regreso_io_sleep struct_a_serializar);
void deserializarRegresoIOSleep(t_buffer *buffer, t_regreso_io_sleep* struct_donde_deserializo);

void serializarSolicitudIoStdin(t_buffer *buffer, t_solicitud_io_stdin struct_a_serializar);
void deserializarSolicitudIoStdin(t_buffer *buffer, t_solicitud_io_stdin* struct_donde_deserializo);
void serializarRegresoIOStdin(t_buffer *buffer, t_regreso_io_stdin struct_a_serializar);
void deserializarRegresoIOStdin(t_buffer *buffer, t_regreso_io_stdin* struct_donde_deserializo);

void serializarRegresoIOStdout(t_buffer *buffer, t_regreso_io_stdout struct_a_serializar);
void deserializarRegresoIOStdout(t_buffer *buffer, t_regreso_io_stdout* struct_donde_deserializo);
void serializarSolicitudIoStdout(t_buffer *buffer, t_solicitud_io_stdout struct_a_serializar);
void deserializarSolicitudIoStdout(t_buffer *buffer, t_solicitud_io_stdout* struct_donde_deserializo);

#endif