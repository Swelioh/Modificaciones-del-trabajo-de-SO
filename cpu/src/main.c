#include "../utils.h"

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler, int conexion_stick, int conexion_memory)
{
    liberar_conexion(conexion_scheduler);
    liberar_conexion(conexion_stick);
    liberar_conexion(conexion_memory);
    log_destroy(logger);
    config_destroy(config);
}



int main(int argc, char* argv[]) {
    t_log* logger = log_create("modulo_cpu.log", "modulo_cpu", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_scheduler;
    char* puerto_memory_stick;
    char* puerto_kernel_memory;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Identificador]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Archivos de Config
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(config, "IP", &ip);
    get_string_from_config(config, "PUERTO_KERNEL_SCHEDULER", &puerto_kernel_scheduler);
    get_string_from_config(config, "PUERTO_MEMORY_STICK", &puerto_memory_stick);
    get_string_from_config(config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);

    log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
	log_info(logger, "PUERTO_MEMORY_STICK: %s", puerto_memory_stick);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);

    // CONEXION CLIENTE CON KERNEL SCHEDULER
	int conexion_scheduler = crear_conexion(logger, ip, puerto_kernel_scheduler);
    log_info(logger, "> Modulo CPU Conectado a Scheduler");

    // CONEXION CLIENTE CON MEMORY STICK
    int conexion_stick = crear_conexion(logger, ip, puerto_memory_stick);
    log_info(logger, "> Modulo CPU Conectado a Memory Stick");

    // CONEXION CLIENTE CON KERNEL MEMORY
	int conexion_memory = crear_conexion(logger, ip, puerto_kernel_memory);
    log_info(logger, "> Modulo CPU Conectado a Kernel Memory");

    //--------------------------------------------DESARROLLO CPU
    int pid_actual = 1; // Mock
    bool procesando = true;
    t_registros registros = {0}; // Setea todos los campos en cero automáticamente

    char* programa[] = {
    "SET AX 10",
    "SET BX 20",
    "SUM AX BX",
    "JNZ AX 4", // Ejemplo de salto
    "EXIT"
    };

while (1) {
    log_info(logger, "CPU esperando proceso del Kernel...");
    
    // La CPU se bloquea acá hasta que el Kernel mande algo
    int cod_op = recibir_operacion(conexion_scheduler);
    
    switch (cod_op) {
        case NUEVO_PROCESO:
            //  nos mandaron un PID, lo leemos
            pid_actual = recibir_pid(conexion_scheduler);
            log_info(logger, "El Kernel me asignó el PID: %d", pid_actual);
            
            // el Contexto a la Memoria
            solicitar_contexto_a_memoria(conexion_memory, pid_actual);

            registros = recibir_contexto(conexion_memory);
            log_info(logger, "Contexto recibido. PC inicial: %d", registros.PC);
            // TODO: Acá arranca while(procesando) { Fetch, Decode, Execute... }
            procesando = true;
            while(procesando) {
                
                //FETCH
               char* instruccion_proxima = fetch_instruccion(conexion_memory, pid_actual, registros.PC);
                
                log_info(logger, "## PID: %d - FETCH - Program Counter: %d", pid_actual, registros.PC);
                
                //TODO: EXECUTE (HECHO, HAY QUE VER SI ESTA BIEN)
                // DECODE
               
                char operacion[20] = {0};
                char parametro1[20] = {0};
                char parametro2[20] = {0};
                
                //  palabras del string (ej: "SET" "AX" "10")
                // ¡Que pasa con EXIT? DUDA DE IMPLEMENTACION??
                sscanf(instruccion_proxima, "%s %s %s", operacion, parametro1, parametro2);
                // EXECUTE
                if (strcmp(operacion, "SET") == 0) {
                    // SET (Registro, Valor)
                    int valor = atoi(parametro2); // Convertimos el string del número a un int real
                    
                    // Mapeamos el string del registro a nuestro t_registros
                    if (strcmp(parametro1, "AX") == 0) registros.AX = (uint8_t)valor;
                    else if (strcmp(parametro1, "BX") == 0) registros.BX = (uint8_t)valor;
                    else if (strcmp(parametro1, "CX") == 0) registros.CX = (uint8_t)valor;
                    else if (strcmp(parametro1, "DX") == 0) registros.DX = (uint8_t)valor;
                    else if (strcmp(parametro1, "EAX") == 0) registros.EAX = (uint32_t)valor;
                    else if (strcmp(parametro1, "EBX") == 0) registros.EBX = (uint32_t)valor;
                    else if (strcmp(parametro1, "ECX") == 0) registros.ECX = (uint32_t)valor;
                    else if (strcmp(parametro1, "EDX") == 0) registros.EDX = (uint32_t)valor;

                   
                    log_info(logger, "## PID: %d - Ejecutando: %s - [%s, %s]", pid_actual, operacion, parametro1, parametro2);
                    
                    registros.PC++; 
                } 
                else if (strcmp(operacion, "SUM") == 0) {
                    // SUM (Destino, Origen) - Asumimos AX y BX para tu prueba
                    if (strcmp(parametro1, "AX") == 0 && strcmp(parametro2, "BX") == 0) {
                        registros.AX += registros.BX;
                    }
                    
                    log_info(logger, "## PID: %d - Ejecutando: %s - [%s, %s]", pid_actual, operacion, parametro1, parametro2);
                    registros.PC++;
                }
                else if (strcmp(operacion, "JNZ") == 0) {
                    // JNZ (Registro, Salto)
                    uint32_t salto = (uint32_t)atoi(parametro2);
                    uint32_t valor_registro = 0;

                    if (strcmp(parametro1, "AX") == 0) valor_registro = registros.AX;
                    else if (strcmp(parametro1, "EAX") == 0) valor_registro = registros.EAX;

                    log_info(logger, "## PID: %d - Ejecutando: %s - [%s, %s]", pid_actual, operacion, parametro1, parametro2);

                    if (valor_registro != 0) {
                        registros.PC = salto; // Saltamos (no sumamos 1)
                    } else {
                        registros.PC++;       // Avanzamos normal
                    }
                }
                else if (strcmp(operacion, "EXIT") == 0) {
                    log_info(logger, "## PID: %d - Ejecutando: %s", pid_actual, operacion);
                    procesando = false; // Rompe el ciclo
                }
                else {
                    log_error(logger, "PID: %d - Instruccion desconocida: %s", pid_actual, operacion);
                    procesando = false; // Rompemos por seguridad
                }
                //LIBERO MEMORIA DE LA INSTRUCCION
                free(instruccion_proxima);

                
                // 4. CHECK INTERRUPT

            }

            break;
            
        case -1:
            log_error(logger, "El Kernel se desconectó. Apagando CPU.");
            return EXIT_FAILURE;
            
        default:
            log_warning(logger, "Operacion desconocida del Kernel");
            break;
    }
}


    liberar_recursos(logger, config, conexion_scheduler, conexion_stick, conexion_memory);

    return 0;
}
