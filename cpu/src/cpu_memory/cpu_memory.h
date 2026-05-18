#ifndef CPU_MEMORY_H_
#define CPU_MEMORY_H_

#include <utils/utils.h>
#include <serializacion/scheduler_cpu/serializacionesScheduler_Cpu.h>
#include <utils/src/variables_globales>
#include <cpu_utils/cpu_utils.h>

void solicitar_contexto_a_memoria(int socket_memoria, int pid);
t_registros recibir_contexto(int socket_memoria);
char* fetch_instruccion(int socket_memoria, int pid, uint32_t pc);
void enviar_contexto_a_memoria(int socket_memoria, int pid, t_registros* registros);



#endif