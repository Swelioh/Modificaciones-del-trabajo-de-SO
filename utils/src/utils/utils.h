#ifndef UTILS_H_
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>

// ESTRUCTURAS
typedef enum
{
	MENSAJE,
	PAQUETE
}op_code;

typedef struct
{
	uint32_t size; // TamaÃ±o del payload
    uint32_t offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


//Conexiones
t_paquete* crear_paquete(op_code codigo);
void crear_buffer(t_paquete* paquete);
void agregar_int32_al_buffer(t_buffer *buffer, int32_t variable);
int32_t leer_int32_del_buffer(t_buffer *buffer);
void agregar_string_al_buffer(t_buffer *buffer, uint32_t largo, char *string);
char *leer_string_del_buffer(t_buffer *buffer);
void eliminar_paquete(t_paquete* paquete);
t_paquete* recibir_paquete_completo(int fd);

// Cliente
int crear_conexion(char* ip, char* puerto);
void manejar_desconexion(int fd_cliente, int tipo_cliente);
void liberar_conexion(int socket_cliente);

// Servidor
int iniciar_servidor(t_log* logger, char* puerto);
int esperar_cliente(int socket_servidor, t_log * logger);
int recibir_operacion(int socket_cliente);

// Config
t_config* iniciar_config(void);
void get_string_from_config(t_config* config, char* clave, char** valor);
void get_int_from_config(t_config* config, char* clave, int* valor);
void get_array_from_config(t_config* config, char* clave, char*** valor);

#endif