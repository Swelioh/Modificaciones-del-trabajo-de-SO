#ifndef SERIALIZACION_H_
#define SERIALIZACION_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/string.h>

// MISCS ---------------------------------------------------------------------------
t_paquete* crear_paquete(op_code codigo);
void crear_buffer(t_paquete* paquete);
void eliminar_paquete(t_paquete* paquete);
void agregar_uint32_al_buffer(t_buffer *buffer, uint32_t variable);
uint32_t leer_uint32_del_buffer(t_buffer *buffer);
void agregar_int32_al_buffer(t_buffer *buffer, int32_t variable);
int32_t leer_int32_del_buffer(t_buffer *buffer);
void agregar_uint8_al_buffer(t_buffer *buffer, uint8_t variable);
uint8_t leer_uint8_del_buffer(t_buffer *buffer);
void agregar_string_al_buffer(t_buffer *buffer, uint32_t largo, char *string);
char *leer_string_del_buffer(t_buffer *buffer);
// Funciones para enviar paquetes
t_paquete* armar_paquete(op_code codigo, void* struct_con_mensaje);
void* serializar_paquete(t_paquete* paquete, int bytes);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
// Funciones para serializar y deserializar
void serializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu struct_a_serializar);
void deserializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu* struct_donde_deserializo);

#endif