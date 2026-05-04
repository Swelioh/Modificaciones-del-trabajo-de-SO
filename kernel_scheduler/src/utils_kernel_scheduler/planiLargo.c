#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "planiLargo.h"

int pid_actual = 0;

void *hilo_largo_plazo(void *arg) {
    (void)arg;

    while (1) {
        sem_wait(&semaforo_hay_propuestas_pcb);

        pthread_mutex_lock(&listaEstadosNew.mutexLista);
        t_propuesta_PCB *propuesta = queue_pop(listaEstadosNew.ProcesosDelEstado);
        pthread_mutex_unlock(&listaEstadosNew.mutexLista);

        if (EnviarCrearProceso(PCB_PROP) == -1) {
            log_error(logger, "No encontró el path %s", propuesta->path);
            free(propuesta);
            continue;
        }

        t_pcb *pcb = crearPCB(propuesta->pid, propuesta->prioridad)

        pthread_mutex_lock(&listaEstadosReady.mutexLista);
        queue_push(listaEstadosReady.ProcesosDelEstado, pcb);
        pthread_mutex_unlock(&listaEstadosReady.mutexLista);

        sem_post(&semaforo_hay_pcb_ready);
    }

    return NULL;
}

t_pcb *crear_pcb(int prioridad, char *path, t_estado estado) {
    t_pcb *pcb = malloc(sizeof(t_pcb));
    if (!pcb) { log_error("malloc pcb"); return NULL; }

    pcb->pid                = pid_actual++;
    strncpy(propuesta->path, path, 255);
    propuesta->path[255] = '\0';
    pcb->prioridad          = prioridad;
    pcb->prioridad_original = prioridad;
    pcb->estado             = estado;

    return pcb;
}

int EnviarCrearProceso(t_propuesta_PCB *propuesta) {
    // Enviar
    t_paquete *paquete = crear_paquete(INICIAR_PROCESO);
    agregar_int32_al_buffer(paquete->buffer, propuesta->pid);
    agregar_string_al_buffer(paquete->buffer, strlen(propuesta->path), propuesta->path);
    enviar_paquete(paquete, fd_memoria);
    eliminar_paquete(paquete);

    // Recibir respuesta. TODO: Que hay que esperar de KM?  
    t_paquete *respuesta = recibir_paquete_completo(fd_memoria);
    if (!respuesta) return -1;

    int resultado = respuesta->codigo_operacion;
    eliminar_paquete(respuesta);

    return (resultado == PROCESO_CREADO) ? 0 : -1;
}