#ifndef CPU_UTILS_H_
#define CPU_UTILS_H_
#include <utils/utils.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <commons/log.h>

// 1. Los códigos para que Kernel y CPU hablen el mismo idioma
typedef enum {
    NUEVO_PROCESO,       // El Kernel te manda un PID para ejecutar
    DESALOJO_PROCESO,    // La CPU le devuelve el PID al Kernel
    INTERRUPCION,        // El Kernel le dice a la CPU que frene
    MENSAJE,              // Para mandar strings simples (como el Handshake)
    PEDIR_CONTEXTO,      // CPU le dice a Memoria: "Dame los registros del PID X"
    CONTEXTO_ACTUALIZADO, // Memoria le responde a CPU con los registros
    PEDIR_INSTRUCCION  //Para el Fetch
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

typedef struct {
    uint32_t PC;  
    uint8_t AX, BX, CX, DX; 
    uint32_t EAX, EBX, ECX, EDX; 
    uint32_t SI, DI; 
} t_registros;

int recibir_operacion(int socket_cliente);
int recibir_pid(int socket_cliente);
void solicitar_contexto_a_memoria(int socket_memoria, int pid);
t_registros recibir_contexto(int socket_memoria);
char* fetch_instruccion(int socket_memoria, int pid, uint32_t pc);

#endif