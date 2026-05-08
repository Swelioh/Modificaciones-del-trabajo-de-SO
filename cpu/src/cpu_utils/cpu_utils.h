#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_
#include <utils/utils.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/log.h>
#include <commons/config.h>
#include <variables_globales/variables_globales.h>
#include <serializacion/scheduler_cpu/serializacionesScheduler_Cpu.h>
#include <serializacion/cpu_kernel_memory/serializaciones_cpu_kernel_memory.h>
#include "variablesGlobales.h"

typedef struct {
    uint32_t PC;  
    uint8_t AX, BX, CX, DX; 
    uint32_t EAX, EBX, ECX, EDX; 
    uint32_t SI, DI; 
} t_registros;


int recibir_codigo_de_operacion(int socket);
uint32_t  recibir_pid_del_scheduler(int socket);
void solicitar_contexto_a_memoria(int socket_memoria, int pid);
t_registros recibir_contexto(int socket_memoria);
char* fetch_instruccion(int socket_memoria, int pid, uint32_t pc);
uint32_t leer_registro(t_registros* registros, char* nombre);
void escribir_registro(t_registros* registros, char* nombre, uint32_t valor);

void enviar_paquete_a_modulo(t_paquete* paquete, int socket_modulo);
t_paquete* recibir_paquete_de_modulo(int socket_modulo);
void liberar_cpu();

#endif