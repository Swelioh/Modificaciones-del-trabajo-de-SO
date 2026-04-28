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

// Cliente
// int crear_conexion(t_log* logger, char* ip, char* puerto);//viejo
int crear_conexion(t_log* logger, char* ip, char* puerto);
// TODO: ESTE VA EN SCHEDULER
// void manejar_desconexion(int fd_cliente, int tipo_cliente);
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

#endif