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

void get_string_from_config(t_config* config, char* clave, char** valor);
void get_int_from_config(t_config* config, char* clave, int* valor);
void crear_buffer(t_paquete* paquete);
t_paquete* crear_paquete(void);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void manejar_paquete(int conexion);
int recibir_operacion(int socket_cliente);
t_list* recibir_paquete(int socket_cliente);

#endif