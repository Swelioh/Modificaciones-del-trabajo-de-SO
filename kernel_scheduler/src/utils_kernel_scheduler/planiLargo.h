#ifndef PL_H_
#define PL_H_

#include <stdint.h> 
#include <utils/utils.h>
#include<pthread.h>      // Para usar semaforos
#include "variablesGlobales.h" // Para poder conocer variables globales en el config

t_pcb* crear_pcb(int pid, int prioridad, t_estado estado);
void *hilo_largo_plazo(void *arg);

#endif