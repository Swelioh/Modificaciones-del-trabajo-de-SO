#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include<netdb.h>
#include<variables_globales/variables_globales.h> // Para saber que es un codigo_operacion

// ********************************************************************************
//                            ESTRUCTURAS DE PAQUETES
// *********************************************************************************
typedef struct
{
	uint32_t size; // Tamaño del payload
    uint32_t offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer;

typedef struct
{
	codigo_operacion codigo_operacion; // Codigo para reconocer el mensaje
	t_buffer* buffer; // Buffer con informacion serializada
} t_paquete;


#endif