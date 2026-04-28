#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include<netdb.h>

// ********************************************************************************
//                            ESTRUCTURAS DE PAQUETES
// *********************************************************************************
typedef enum // TODO: BORRAR PORQUE YA ESTA EL ENUM PARA EL HANDSHAKE EN UTILS VARIABLES GLOBALES
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
// TODO DEBERIA ESTAR EN SCHEDULER SI NO SE COMPARTE
//Modeliza el mensaje que envia CPU al SCHEDULER avisando que se dispone de una CPU nueva para el procesamiento de datos
typedef struct
{
	uint32_t id_cpu;
} t_ingreso_cpu;

#endif