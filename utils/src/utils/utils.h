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
#include <stdint.h>     
#include <stdbool.h>

// Cliente
int crear_conexion(t_log* logger, char* ip, char* puerto);
// TODO: ESTE VA EN SCHEDULER
void liberar_conexion(int socket_cliente);

// Servidor
int iniciar_servidor(t_log* logger, char* puerto);
int esperar_cliente(int socket_servidor, t_log * logger);
int recibir_operacion(int socket_cliente);

// Config
t_config* abrirConfig(char* pathAlConfig);
void get_string_from_config(t_config* config, char* clave, char** valor);
void get_int_from_config(t_config* config, char* clave, int* valor);
void get_array_from_config(t_config* config, char* clave, char*** valor);

typedef enum {
    SET, SUM, SUB, JNZ, COPY_MEM, MOV_IN, MOV_OUT, // Instrucciones de CPU
    SLEEP, STDIN, STDOUT, MUTEX_CREATE, MUTEX_LOCK, MUTEX_UNLOCK, 
    MEM_ALLOC, MEM_FREE, INIT_PROC, EXIT // Syscalls
} op_code_instruccion;

#endif