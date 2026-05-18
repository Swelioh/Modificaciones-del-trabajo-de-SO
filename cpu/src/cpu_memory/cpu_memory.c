#include "cpu_memory.h"

void solicitar_contexto_a_memoria(int socket_memoria, int pid) {
    t_stream_pedido_contexto pedido;
    pedido.pid = (uint32_t)pid;
    
    t_paquete* paquete = armar_paquete_cpu_kernel_memory(PEDIR_CONTEXTO, &pedido);
    enviar_paquete_a_modulo(paquete, socket_memoria); // Ya libera la memoria adentro
}

t_registros recibir_contexto(int socket_memoria) {
    t_registros registros_recibidos;
    uint32_t size;
    int cod_op;

    recv(socket_memoria, &cod_op, sizeof(int), MSG_WAITALL);
    recv(socket_memoria, &size, sizeof(uint32_t), MSG_WAITALL);
    recv(socket_memoria, &registros_recibidos, size, MSG_WAITALL);

    return registros_recibidos;
}

char* fetch_instruccion(int socket_memoria, int pid, uint32_t pc) {
    // 1. ENVIAR PEDIDO
    t_stream_prox_instruccion pedido;
    pedido.pid = (uint32_t)pid;
    pedido.pc = pc;

    t_paquete* paquete_pedido = armar_paquete_cpu_kernel_memory(PEDIR_PROX_INSTRUCCION, &pedido);
    enviar_paquete_a_modulo(paquete_pedido, socket_memoria);

    // 2. ESPERAR Y RECIBIR RESPUESTA
    t_paquete* paquete_rta = recibir_paquete_de_modulo(socket_memoria);

    if (paquete_rta->codigo_operacion == RESPUESTA_PROX_INSTRUCCION) {
        t_stream_rta_prox_instruccion respuesta;
        desarmar_paquete_cpu_kernel_memory(paquete_rta->codigo_operacion, paquete_rta->buffer, &respuesta);
        
        eliminar_paquete(paquete_rta); // Libera todo el buffer
        return respuesta.instruccion; 
    }

    eliminar_paquete(paquete_rta);
    return NULL; 
}

void enviar_contexto_a_memoria(int socket_memoria, int pid, t_registros* registros) {
    // 1. Armás el paquete con tus registros actualizados
    t_paquete* paquete = armar_paquete_cpu_kernel_memory(ACTUALIZAR_CONTEXTO, registros);
    enviar_paquete_a_modulo(paquete, socket_memoria);
    
    // 2. Esperar confirmación de la Memoria (El enunciado suele pedir que la memoria confirme)
    t_paquete* paquete_rta = recibir_paquete_de_modulo(socket_memoria);
    if (paquete_rta->codigo_operacion == CONTEXTO_ACTUALIZADO) {
        log_trace(logger, "Contexto del PID %d actualizado en memoria correctamente.", pid);
    } else {
        log_error(logger, "Fallo al actualizar contexto en memoria.");
    }
    eliminar_paquete(paquete_rta);
}