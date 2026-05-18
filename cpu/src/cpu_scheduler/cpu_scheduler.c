#include "cpu_scheduler.h"

 
// ----------------------------------------------------
// RECIBIR DEL SCHEDULER
// ----------------------------------------------------
int recibir_codigo_de_operacion(int socket_cliente) {
    int cod_op;
    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) {
        return cod_op;
    } else {
        close(socket_cliente);
        return -1; 
    }
}

uint32_t recibir_pid_del_scheduler(int socket_cliente) {
    t_paquete* paquete = recibir_paquete_de_modulo(socket_cliente);
    
    t_nuevo_proceso msj;
    desarmar_paquete_scheduler_cpu(paquete->codigo_operacion, paquete->buffer, &msj);

    eliminar_paquete(paquete); 
    return msj.pid;
}

// ----------------------------------------------------
// DEVOLVER AL SCHEDULER 
// ----------------------------------------------------
void devolver_proceso_a_scheduler(int socket_scheduler, int pid, uint32_t motivo) {
    t_devolucion_cpu devolucion;
    devolucion.pid = (uint32_t)pid;
    devolucion.motivo = motivo; 
    
    t_paquete* paquete = armar_paquete_scheduler_cpu(DEVOLUCION_PROCESO, &devolucion);
    enviar_paquete_a_modulo(paquete, socket_scheduler);
}

void devolver_proceso_a_scheduler_con_parametro(int socket_scheduler, int pid, uint32_t motivo, uint32_t parametro) {
    t_devolucion_con_parametro devolucion;
    devolucion.pid = (uint32_t)pid;
    devolucion.motivo = motivo; 
    devolucion.parametro = parametro; 
    
    t_paquete* paquete = armar_paquete_scheduler_cpu(DEVOLUCION_PARAMETRO, &devolucion);
    enviar_paquete_a_modulo(paquete, socket_scheduler);
}
