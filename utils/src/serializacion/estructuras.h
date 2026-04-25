

#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include<netdb.h>

// ********************************************************************************
//                            ESTRUCTURAS DE PAQUETES
// *********************************************************************************
typedef enum
{
	INGRESO_CPU,
	INGRESO_IO
}op_code;

typedef struct
{
	uint32_t size; // Tamaño del payload
    uint32_t offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer;

typedef struct
{
	op_code codigo_operacion; //Codigo para reconocer el mensaje
	t_buffer* buffer; //Buffer con informacion serializada
} t_paquete;

// ********************************************************************************
//                          ESTRUCTURAS DE MENSAJES
// *********************************************************************************

//Modeliza el mensaje que envia CPU al SCHEDULER avisando que se dispone de una CPU nueva para el procesamiento de datos
typedef struct
{
	uint32_t id_cpu;
} t_ingreso_cpu;

#endif