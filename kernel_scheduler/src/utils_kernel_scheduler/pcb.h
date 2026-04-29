#ifndef PCB_H_
#define PCB_H_

#include <stdint.h> 
#include <utils/utils.h>

typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCK,
    SUSP_READY,
    SUSP_BLOCK,
    EXIT
} t_estado;

typedef struct {
    uint32_t pid;
    t_estado estado;
    int      prioridad;
    int      prioridad_original;
    struct timespec tiempo_llegada;
} t_pcb;

t_pcb* crear_pcb(int pid);

#endif