#include "cpu_utils.h"

int recibir_codigo_de_operacion(int socket_cliente) {
    int cod_op;
    if (recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0) {
        return cod_op;
    } else {
        close(socket_cliente);
        return -1; // Hubo un error o el Kernel se desconectó
    }
} //ESTO YO LA HACE RECIBIR PAQUETE DE MODULO

uint32_t recibir_pid_del_scheduler(int socket_cliente) {
    // Reconstruimos el buffer temporal
    t_buffer* buffer = malloc(sizeof(t_buffer));
    
    // Leemos el tamaño del payload
    recv(socket_cliente, &(buffer->size), sizeof(uint32_t), MSG_WAITALL);
    
    // Leemos los datos reales
    buffer->stream = malloc(buffer->size);
    recv(socket_cliente, buffer->stream, buffer->size, MSG_WAITALL);
    buffer->offset = 0; // CRÍTICO para que lea de cero

    // Usamos tu deserializador
    t_nuevo_proceso msj;
    desarmar_paquete_scheduler_cpu(NUEVO_PROCESO, buffer, &msj);

    // Limpiamos la memoria del buffer crudo
    free(buffer->stream);
    free(buffer);
    
    return msj.pid;
}

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

        // Limpiamos el buffer temporal
        free(buffer->stream);
        free(buffer);

        // Devolvemos el string puro (que luego vas a liberar en el main con free())
        return respuesta.instruccion; 
    }

    return NULL; // En caso de que la Memoria nos mande cualquier otra cosa o se desconecte
}



//EXECUTE 

void escribir_registro(t_registros* registros, char* registro_str, uint32_t valor) {
    // Registros de 8 bits
    if (strcmp(registro_str, "AX") == 0) registros->AX = (uint8_t)valor;
    else if (strcmp(registro_str, "BX") == 0) registros->BX = (uint8_t)valor;
    else if (strcmp(registro_str, "CX") == 0) registros->CX = (uint8_t)valor;
    else if (strcmp(registro_str, "DX") == 0) registros->DX = (uint8_t)valor;
    
    // Registros de 32 bits
    else if (strcmp(registro_str, "EAX") == 0) registros->EAX = valor;
    else if (strcmp(registro_str, "EBX") == 0) registros->EBX = valor;
    else if (strcmp(registro_str, "ECX") == 0) registros->ECX = valor;
    else if (strcmp(registro_str, "EDX") == 0) registros->EDX = valor;
    else if (strcmp(registro_str, "SI") == 0) registros->SI = valor;
    else if (strcmp(registro_str, "DI") == 0) registros->DI = valor;
}

uint32_t leer_registro(t_registros* registros, char* registro_str) {
    // Registros de 8 bits
    if (strcmp(registro_str, "AX") == 0) return (uint32_t)registros->AX;
    if (strcmp(registro_str, "BX") == 0) return (uint32_t)registros->BX;
    if (strcmp(registro_str, "CX") == 0) return (uint32_t)registros->CX;
    if (strcmp(registro_str, "DX") == 0) return (uint32_t)registros->DX;
    
    // Registros de 32 bits
    if (strcmp(registro_str, "EAX") == 0) return registros->EAX;
    if (strcmp(registro_str, "EBX") == 0) return registros->EBX;
    if (strcmp(registro_str, "ECX") == 0) return registros->ECX;
    if (strcmp(registro_str, "EDX") == 0) return registros->EDX;
    if (strcmp(registro_str, "SI") == 0) return registros->SI;
    if (strcmp(registro_str, "DI") == 0) return registros->DI;

    return 0; // Por si llega basura
}


void liberar_cpu(){
    log_destroy(logger);
    config_destroy(config);
}

void enviar_paquete_a_modulo(t_paquete* paquete, int socket_modulo)
{	
    if(enviar_paquete(paquete,socket_modulo)<0){
        liberar_cpu();
        eliminar_paquete(paquete);

        exit(0);
        //TODO: DEFINIR COMO FINALIZAR EJECUCION
    }
    eliminar_paquete(paquete);
}

t_paquete* recibir_paquete_de_modulo(int socket_modulo){	
    t_paquete* paquete=recibir_paquete_completo(socket_modulo);

    if(paquete==NULL){
        liberar_cpu();
        exit(0);
        //TODO: DEFINIR COMO FINALIZAR EJECUCION
    }
    return paquete;
}

