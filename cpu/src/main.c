#include <utils/utils.h>
#include <stdint.h> // Necesario para uint32_t y uint8_t

typedef struct {
    uint32_t PC;  // Program Counter 
    uint8_t AX, BX, CX, DX; // Registros de 1 byte 
    uint32_t EAX, EBX, ECX, EDX; // Registros de 4 bytes 
    uint32_t SI, DI; // Registros de dirección 
} t_registros;

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler, int conexion_stick, int conexion_memory)
{
    liberar_conexion(conexion_scheduler);
    liberar_conexion(conexion_stick);
    liberar_conexion(conexion_memory);
    log_destroy(logger);
    config_destroy(config);
}



int main(int argc, char* argv[]) {
   t_log* logger = log_create("cpu.log", "CPU", 1, LOG_LEVEL_INFO);

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
    get_string_from_config(logger, config, "IP", &ip);
    get_string_from_config(logger, config, "PUERTO_KERNEL_SCHEDULER", &puerto_kernel_scheduler);
    get_string_from_config(logger, config, "PUERTO_MEMORY_STICK", &puerto_memory_stick);
    get_string_from_config(logger, config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);

    log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
	log_info(logger, "PUERTO_MEMORY_STICK: %s", puerto_memory_stick);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);

    // CONEXION CLIENTE CON KERNEL SCHEDULER
	//int conexion_scheduler = crear_conexion(ip, puerto_kernel_scheduler);
    //log_info(logger, "> Modulo CPU Conectado a Scheduler");

    // CONEXION CLIENTE CON MEMORY STICK
    //int conexion_stick = crear_conexion(ip, puerto_memory_stick);
    //log_info(logger, "> Modulo CPU Conectado a Memory Stick");

    // CONEXION CLIENTE CON KERNEL MEMORY
	//int conexion_memory = crear_conexion(ip, puerto_kernel_memory);
    //log_info(logger, "> Modulo CPU Conectado a Kernel Memory");

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


    while (procesando) {
    
    if (registros.PC >= 5) break;
    // 1. FETCH: Buscar la instrucción
    char* instruccion_proxima = programa[registros.PC];
    
    // LOG OBLIGATORIO: ## PID: <PID> FETCH - Program Counter: <PC>
    log_info(logger, "## PID: %d - FETCH - Program Counter: %d", pid_actual, registros.PC);

    // 2. DECODE + 3. EXECUTE
    if (strncmp(instruccion_proxima, "SET", 3) == 0) {
        char reg[5];
        int val;
        sscanf(instruccion_proxima, "SET %s %d", reg, &val);
        
        // Simulación de escritura en registro
        if (strcmp(reg, "AX") == 0) registros.AX = (uint8_t)val;
        if (strcmp(reg, "EAX") == 0) registros.EAX = (uint32_t)val;
        
        log_info(logger, "## PID: %d - Ejecutando: %s", pid_actual, instruccion_proxima);
        registros.PC++; 
    } 
    else if (strncmp(instruccion_proxima, "SUM", 3) == 0) {
        // SUM (Destino, Origen)
        registros.AX += registros.BX;
        log_info(logger, "## PID: %d - Ejecutando: %s", pid_actual, instruccion_proxima);
        registros.PC++;
    }
    else if (strncmp(instruccion_proxima, "EXIT", 4) == 0) {
        log_info(logger, "## PID: %d - Ejecutando: %s", pid_actual, instruccion_proxima);
        procesando = false; // Termina el ciclo para este proceso
    }
    else if (strncmp(instruccion_proxima, "JNZ", 3) == 0) {
    char reg[5];
    int salto;

    sscanf(instruccion_proxima, "JNZ %s %d", reg, &salto);

    uint32_t valor = 0;

    if (strcmp(reg, "AX") == 0) valor = registros.AX;
    if (strcmp(reg, "EAX") == 0) valor = registros.EAX;

    if (valor != 0) {
        registros.PC = salto;
    } else {
        registros.PC++;
    }

    log_info(logger, "## PID: %d - Ejecutando: %s", pid_actual, instruccion_proxima);
}
    // 4. CHECK INTERRUPT
    // chequear si el Kernel mandó algo por el socket de interrupción.
    }

    //liberar_recursos(logger, config, conexion_scheduler, conexion_stick, conexion_memory);

    saludar("cpu");
    return 0;
}
