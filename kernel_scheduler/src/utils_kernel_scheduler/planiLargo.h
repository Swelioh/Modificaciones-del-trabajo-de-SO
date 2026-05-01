#ifndef PL_H_
#define PL_H_

#include <stdint.h> 
#include <utils/utils.h>
#include <kernel_scheduler/src/utils_kernel_scheduler/variablesGlobales.h>

t_pcb* crear_pcb(int pid, int prioridad, t_estado estado);
void *hilo_largo_plazo(void *arg);

#endif