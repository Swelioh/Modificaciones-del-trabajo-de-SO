#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pcb.h"

int pid_actual=0;

t_pcb* crear_pcb(int prioridad)
{
    t_pcb* pcb = (t_pcb*)(malloc(sizeof(t_pcb)));
    pcb->pid = pid_actual;
    pcb->prioridad = prioridad;
    pcb->prioridad_original = prioridad;
    
    pid_actual++;
    return pcb;
}
