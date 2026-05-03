#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commons/log.h>
#include <utils/utils.h>

// 1. Los códigos para que Kernel y CPU hablen el mismo idioma
typedef enum {
    NUEVO_PROCESO,       // El Kernel te manda un PID para ejecutar
    DESALOJO_PROCESO,    // La CPU le devuelve el PID al Kernel
    INTERRUPCION,        // El Kernel le dice a la CPU que frene
    MENSAJE,              // Para mandar strings simples (como el Handshake)
    PEDIR_CONTEXTO,      // CPU le dice a Memoria: "Dame los registros del PID X"
    CONTEXTO_ACTUALIZADO // Memoria le responde a CPU con los registros
} op_code;

// 2. El buffer que guarda el tamaño y los datos crudos
typedef struct {
    uint32_t size;
    void* stream;
} t_buffer;

// 3. El paquete final que viaja por la red
typedef struct {
    op_code codigo_operacion;
    t_buffer* buffer;
} t_paquete;

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