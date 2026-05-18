#ifndef CPU_SCHEDULER_H_
#define CPU_SCHEDULER_H_
#include <utils/utils.h>
#include <serializacion/scheduler_cpu/serializacionesScheduler_Cpu.h>
#include <utils/src/variables_globales>
#include <cpu_utils/cpu_utils.h>

int recibir_codigo_de_operacion(int socket_cliente);
uint32_t recibir_pid_del_scheduler(int socket_cliente);
void devolver_proceso_a_scheduler(int socket_scheduler, int pid, uint32_t motivo);
void devolver_proceso_a_scheduler_con_parametro(int socket_scheduler, int pid, uint32_t motivo, uint32_t parametro);

#endif