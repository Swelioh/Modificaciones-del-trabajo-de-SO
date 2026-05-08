#ifndef CPU_KERNEL_MEMORY_SERIALIZACION_H_
#define CPU_KERNEL_MEMORY_SERIALIZACION_H_

// #include <unistd.h>
#include <serializacion/estructuras.h> // La necesito para poder saber que es un paquete o un buffer
#include <serializacion/serializacion.h> // La necesito porque tiene las funciones de añadir variables al buffer

// ********************************************************************************
//                          ESTRUCTURAS DE MENSAJES
// *********************************************************************************

typedef struct {   
	uint32_t pid;
    uint32_t pc;
} t_stream_prox_instruccion; //Modeliza el mensaje que envia CPU a KERNEL MEMORY asociado a PEDIR_PROX_INSTRUCCION

typedef struct {   
    int32_t largo_instruccion;  
    char* instruccion;  
} t_stream_rta_prox_instruccion; //Modeliza el mensaje de respuesta que envia KERNEL MEMORY a CPU asociado a PEDIR_PROX_INSTRUCCION

typedef struct
{
	uint32_t pid;
} t_stream_pedido_contexto; // CPU le dice a Memoria: "Dame el contexto del PID X"



// ********************************************************************************
//                             ESTRUCTURAS GLOBALES
// *********************************************************************************


// ********************************************************************************
//                          PROTOTIPOS DE SERIALIZACON
// ********************************************************************************

t_paquete* armar_paquete_cpu_kernel_memory(codigo_operacion codigo, void* struct_con_mensaje);
void desarmar_paquete_cpu_kernel_memory(codigo_operacion codigo, t_buffer* buffer, void* struct_con_mensaje);

void serializar_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_prox_instruccion struct_a_serializar);
void deserializar_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_prox_instruccion* struct_donde_deserializo);

void serializar_rta_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_rta_prox_instruccion struct_a_serializar);
void deserializar_rta_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_rta_prox_instruccion* struct_donde_deserializo);

void serializar_pedido_contexto_cpu_kernel_memory(t_buffer *buffer, t_stream_pedido_contexto struct_a_serializar);
void deserializar_pedido_contexto_cpu_kernel_memory(t_buffer *buffer, t_stream_pedido_contexto* struct_donde_deserializo);

#endif