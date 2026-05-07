#include "cpu_utils.h"

int recibir_codigo_de_operacion(int socket_cliente) {
    int cod_op;
    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) {
        return cod_op;
    } else {
        close(socket_cliente);
        return -1; // Hubo un error o el Kernel se desconectó
    }
}

int recibir_pid(int socket_cliente) {
    int pid;
    uint32_t size;
    
    // Primero recibimos cuánto pesa el dato (el size del buffer)
    recv(socket_cliente, &size, sizeof(uint32_t), MSG_WAITALL);
    // Ahora recibimos el dato real (el stream)
    recv(socket_cliente, &pid, size, MSG_WAITALL);
    
    return pid;
} //VER DE CAMBIAR

void solicitar_contexto_a_memoria(int socket_memoria, int pid) {
    // Llenamos la estructura definida en el .h
    t_stream_pedido_contexto pedido;
    pedido.pid = (uint32_t)pid;

    
    t_paquete* paquete = armar_paquete_cpu_kernel_memory(PEDIR_CONTEXTO, &pedido);
    
    // Enviado y limpiado
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);
}

t_registros recibir_contexto(int socket_memoria) {
    t_registros registros_recibidos;
    uint32_t size;
    int cod_op;

    // 1. Esperamos el código de operación (debería ser CONTEXTO_ACTUALIZADO)
    recv(socket_memoria, &cod_op, sizeof(int), MSG_WAITALL);
    
    // 2. Recibimos el tamaño del payload (debería ser sizeof(t_registros))
    recv(socket_memoria, &size, sizeof(uint32_t), MSG_WAITALL);
    
    // 3. Recibimos el struct completo directo a nuestra variable
    recv(socket_memoria, &registros_recibidos, size, MSG_WAITALL);

    return registros_recibidos;
}


char* fetch_instruccion(int socket_memoria, int pid, uint32_t pc) {
    // ==========================================
    // ENVIAR PEDIDO A LA MEMORIA
    // ==========================================
    t_stream_prox_instruccion pedido;
    pedido.pid = (uint32_t)pid;
    pedido.pc = pc;

    t_paquete* paquete_pedido = armar_paquete_cpu_kernel_memory(PEDIR_PROX_INSTRUCCION, &pedido);
    enviar_paquete(paquete_pedido, socket_memoria);
    eliminar_paquete(paquete_pedido);

    // ==========================================
    //  ESPERAR Y RECIBIR LA RESPUESTA
    // ==========================================
    int cod_op;
    recv(socket_memoria, &cod_op, sizeof(int), MSG_WAITALL);

    if (cod_op == RESPUESTA_PROX_INSTRUCCION) {
        // Reconstruccion del t_buffer crudo que viajó por la red
        t_buffer* buffer = malloc(sizeof(t_buffer));
        recv(socket_memoria, &(buffer->size), sizeof(uint32_t), MSG_WAITALL);
        
        buffer->stream = malloc(buffer->size);
        recv(socket_memoria, buffer->stream, buffer->size, MSG_WAITALL);
        buffer->offset = 0; // CRÍTICO: Para que tu función lea desde el principio

        //Deserializador
        t_stream_rta_prox_instruccion respuesta;
        desarmar_paquete_cpu_kernel_memory(cod_op, buffer, &respuesta);

        // C. Limpiamos el buffer temporal
        free(buffer->stream);
        free(buffer);

        // Devolvemos el string puro (que luego vas a liberar en el main con free())
        return respuesta.instruccion; 
    }

    return NULL; // En caso de que la Memoria nos mande cualquier otra cosa o se desconecte
}