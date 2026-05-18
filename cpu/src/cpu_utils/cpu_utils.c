#include "cpu_utils.h"

// ==========================================
// GESTION DE ERRORES Y PAQUETES
// ==========================================
void liberar_cpu() {
    log_destroy(logger);
    config_destroy(config);
    printf("Apagando CPU de emergencia.\n");
}

void enviar_paquete_a_modulo(t_paquete* paquete, int socket_modulo) {	
    if(enviar_paquete(paquete, socket_modulo) < 0) {
        liberar_cpu();
        eliminar_paquete(paquete);
        exit(EXIT_FAILURE);
    }
    eliminar_paquete(paquete);
}

t_paquete* recibir_paquete_de_modulo(int socket_modulo) {	
    t_paquete* paquete = recibir_paquete_completo(socket_modulo);
    if(paquete == NULL) {
        liberar_cpu();
        exit(EXIT_FAILURE);
    }
    return paquete;
}

int recibir_codigo_de_operacion(int socket_cliente) {
    int cod_op;
    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) {
        return cod_op;
    } else {
        close(socket_cliente);
        return -1; 
    }
}

// ==========================================
// FLUJO DE LA CPU
// ==========================================
uint32_t recibir_pid_del_scheduler(int socket_cliente) {
    // ¡Usamos tu deserializador abstracto!
    t_paquete* paquete = recibir_paquete_de_modulo(socket_cliente);
    
    t_nuevo_proceso msj;
    desarmar_paquete_scheduler_cpu(paquete->codigo_operacion, paquete->buffer, &msj);

    eliminar_paquete(paquete); // La funcion en serializacion.c ya hace todo el free()
    return msj.pid;
}

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

// ==========================================
// EXECUTE: REGISTROS
// ==========================================
void escribir_registro(t_registros* registros, char* registro_str, uint32_t valor) {
    if (strcmp(registro_str, "AX") == 0) registros->AX = (uint8_t)valor;
    else if (strcmp(registro_str, "BX") == 0) registros->BX = (uint8_t)valor;
    else if (strcmp(registro_str, "CX") == 0) registros->CX = (uint8_t)valor;
    else if (strcmp(registro_str, "DX") == 0) registros->DX = (uint8_t)valor;
    else if (strcmp(registro_str, "EAX") == 0) registros->EAX = valor;
    else if (strcmp(registro_str, "EBX") == 0) registros->EBX = valor;
    else if (strcmp(registro_str, "ECX") == 0) registros->ECX = valor;
    else if (strcmp(registro_str, "EDX") == 0) registros->EDX = valor;
    else if (strcmp(registro_str, "SI") == 0) registros->SI = valor;
    else if (strcmp(registro_str, "DI") == 0) registros->DI = valor;
}

uint32_t leer_registro(t_registros* registros, char* registro_str) {
    if (strcmp(registro_str, "AX") == 0) return (uint32_t)registros->AX;
    if (strcmp(registro_str, "BX") == 0) return (uint32_t)registros->BX;
    if (strcmp(registro_str, "CX") == 0) return (uint32_t)registros->CX;
    if (strcmp(registro_str, "DX") == 0) return (uint32_t)registros->DX;
    if (strcmp(registro_str, "EAX") == 0) return registros->EAX;
    if (strcmp(registro_str, "EBX") == 0) return registros->EBX;
    if (strcmp(registro_str, "ECX") == 0) return registros->ECX;
    if (strcmp(registro_str, "EDX") == 0) return registros->EDX;
    if (strcmp(registro_str, "SI") == 0) return registros->SI;
    if (strcmp(registro_str, "DI") == 0) return registros->DI;
    return 0; 
}
