#include <utils.h>

int recibir_operacion(int socket_cliente) {
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
}

void solicitar_contexto_a_memoria(int socket_memoria, int pid) {
    op_code codigo = PEDIR_CONTEXTO;
    uint32_t size = sizeof(int);
    
    // 1. Enviamos el código de operación
    send(socket_memoria, &codigo, sizeof(op_code), 0);
    // 2. Enviamos el tamaño del payload
    send(socket_memoria, &size, sizeof(uint32_t), 0);
    // 3. Enviamos el dato (el PID)
    send(socket_memoria, &pid, sizeof(int), 0);
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