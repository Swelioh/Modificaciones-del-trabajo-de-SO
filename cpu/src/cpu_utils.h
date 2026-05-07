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
#include <variables_globales/variables_globales.h>
#include <serializacion/scheduler_cpu/serializacionesScheduler_Cpu.h>
#include <serializacion/cpu_kernel_memory/serializaciones_cpu_kernel_memory.h>


//  El buffer que guarda el tamaño y los datos crudos
typedef struct {
    uint32_t PC;  
    uint8_t AX, BX, CX, DX; 
    uint32_t EAX, EBX, ECX, EDX; 
    uint32_t SI, DI; 
} t_registros;

int recibir_operacion(int socket_cliente);
int recibir_pid(int socket_cliente);
void solicitar_contexto_a_memoria(int socket_memoria, int pid);
t_registros recibir_contexto(int socket_memoria);
char* fetch_instruccion(int socket_memoria, int pid, uint32_t pc);

#endif