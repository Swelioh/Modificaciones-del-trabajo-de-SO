#include "instrucciones.h"

bool ejecutar_instruccion(char** instruccion, t_registros* registros, int pid, int conexion_scheduler, int conexion_memory, t_log* logger) {
    char* op = instruccion[0];

    if (strcmp(op, "SET") == 0) {
        instruccion_set(instruccion, registros, pid, logger);
        return true; 
    } 
    else if (strcmp(op, "SUM") == 0) {
        instruccion_sum(instruccion, registros, pid, logger);
        return true;
    }
    else if (strcmp(op, "JNZ") == 0) {
        instruccion_jnz(instruccion, registros, pid, logger);
        return true;
    }
    else if (strcmp(op, "EXIT") == 0) {
        instruccion_exit(pid, registros, conexion_scheduler, conexion_memory, logger);
        return false; // Frena el while(procesando) en el main
    }
    // TODO: Agregar SLEEP, MUTEX_LOCK, etc. (Devuelven false)
    else {
        log_error(logger, "## PID: %d - Instrucción desconocida: %s", pid, op);
        return false;
    }
}

void instruccion_set(char** instruccion, t_registros* registros, int pid, t_log* logger) {
    char* parametro1 = instruccion[1];
    char* parametro2 = instruccion[2];
    
    int valor = atoi(parametro2);
    
    // Escribir en el registro (asumiendo que tenés tu función escribir_registro)
    escribir_registro(registros, parametro1, valor);
    
    log_info(logger, "## PID: %d - Ejecutando: SET - [%s, %s]", pid, parametro1, parametro2);
    registros->PC++;
}

void instruccion_exit(int pid, t_registros* registros, int conexion_scheduler, int conexion_memory, t_log* logger) {
    log_info(logger, "## PID: %d - Ejecutando: EXIT", pid);
    
    // Guardamos contexto en memoria
    enviar_contexto_a_memoria(conexion_memory, pid, registros);
    
    // Avisamos al scheduler el motivo
    devolver_proceso_a_scheduler(conexion_scheduler, pid, MOTIVO_EXIT);
}

void instruccion_sum(char** instruccion, t_registros* registros, int pid, t_log* logger) {
    // 1. Extraemos los parámetros del array (sabemos que SUM tiene 2 parámetros)
    char* reg_destino = instruccion[1];
    char* reg_origen = instruccion[2];

    // 2. Leemos los valores numéricos de los registros
    uint32_t valor_destino = leer_registro(registros, reg_destino);
    uint32_t valor_origen = leer_registro(registros, reg_origen);

    // 3. Ejecutamos la lógica de la suma
    uint32_t resultado = valor_destino + valor_origen;

    // 4. Guardamos el resultado en el registro destino
    escribir_registro(registros, reg_destino, resultado);

    // 5. Log obligatorio por la cátedra (Ej: "## PID: 1 - Ejecutando: SUM - AX BX")
    log_info(logger, "## PID: %d - Ejecutando: SUM - %s %s", pid, reg_destino, reg_origen);

    // 6. Avanzamos el Program Counter a la siguiente instrucción
    registros->PC++;
}

void instruccion_jnz(char** instruccion, t_registros* registros, int pid, t_log* logger) {
    // 1. Extraemos los parámetros
    char* registro = instruccion[1];
    char* salto_str = instruccion[2];

    // 2. Obtenemos el valor a evaluar y el salto en formato numérico
    uint32_t valor_registro = leer_registro(registros, registro);
    uint32_t instruccion_salto = (uint32_t)atoi(salto_str);

    // 3. Log obligatorio de ejecución
    log_info(logger, "## PID: %d - Ejecutando: JNZ - %s %s", pid, registro, salto_str);

    // 4. Lógica del salto
    if (valor_registro != 0) {
        // Si no es cero, el Program Counter salta directamente a la instrucción deseada.
        // Acá NO se suma 1, porque queremos ir exactamente a 'instruccion_salto'
        registros->PC = instruccion_salto;
    } else {
        // Si es cero, la condición no se cumple y la CPU sigue normalmente
        registros->PC++;
    }
}