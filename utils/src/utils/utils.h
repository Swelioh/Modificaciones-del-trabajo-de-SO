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
#include <stdint.h>     
#include <stdbool.h>
/**
* @brief Imprime un saludo por consola
* @param quien Módulo desde donde se llama a la función
* @return No devuelve nada
*/
void saludar(char* quien);

// Funciones para CLIENTE

int crear_conexion(char* ip, char* puerto);
void liberar_conexion(int socket_cliente);

// Funciones para SERVIDOR
int iniciar_servidor(t_log* logger, char* puerto);
int esperar_cliente(int socket_servidor, t_log * logger);

//Config
t_config* iniciar_config(void);
void get_string_from_config(t_log* logger, t_config* config, char* clave, char** valor);

typedef enum {
    SET, SUM, SUB, JNZ, COPY_MEM, MOV_IN, MOV_OUT, // Instrucciones de CPU
    SLEEP, STDIN, STDOUT, MUTEX_CREATE, MUTEX_LOCK, MUTEX_UNLOCK, 
    MEM_ALLOC, MEM_FREE, INIT_PROC, EXIT // Syscalls
} op_code_instruccion;

#endif