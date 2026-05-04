#ifndef SCHEDULER_CPU_SERIALIZACION_H_
#define SCHEDULER_CPU_SERIALIZACION_H_

#include <unistd.h>
#include <serializacion/estructuras.h> // La necesito para poder saber que es un paquete o un buffer
#include <serializacion/serializacion.h> // La necesito porque tiene las funciones de a;adir variables al buffer

// ********************************************************************************
//                          ESTRUCTURAS DE MENSAJES
// *********************************************************************************

typedef struct
{
	uint32_t identificador_cpu;
} t_ingreso_cpu; //Modeliza el mensaje que envia CPU al SCHEDULER avisando que se dispone de una CPU nueva para el procesamiento de datos


// ********************************************************************************
//                          PROTOTIPOS DE SERIALIZACON
// ********************************************************************************

t_paquete* armar_paquete_scheduler_cpu(codigo_operacion codigo, void* struct_con_mensaje);
void serializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu struct_a_serializar);
void deserializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu* struct_donde_deserializo);

#endif



