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
