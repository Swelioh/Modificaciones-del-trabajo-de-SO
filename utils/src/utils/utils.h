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

#endif