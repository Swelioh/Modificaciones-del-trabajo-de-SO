#include <utils/utils.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_memory_stick, int conexion_kernel_memory)
{
    liberar_conexion(conexion_memory_stick);
    liberar_conexion(conexion_kernel_memory);
    log_destroy(logger);
    config_destroy(config);
}

int main(int argc, char* argv[]) {
 
    t_log* logger = log_create("memory_stick.log", "memory_stick", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_memory;
    char* puerto_memory_stick;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Path Proceso Inicial]\n", argv[0]);
        return EXIT_FAILURE;
    }

    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(logger, config, "IP", &ip);
    get_string_from_config(logger, config, "PUERTO_MEMORY_STICK", &puerto_memory_stick);
    get_string_from_config(logger, config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);
	
    log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);
	log_info(logger, "PUERTO_MEMORY_STICK: %s", puerto_memory_stick);
   
    // Conexion para memory
	int conexion_kernel_memory = crear_conexion(ip, puerto_kernel_memory);

	int conexion_memory_stick = iniciar_servidor(logger, puerto_memory_stick);
   
    log_info(logger, "> Memory stick Listo");

    // Esperamos a la CPU
    esperar_cliente(conexion_memory_stick, logger);
    
    liberar_recursos(logger, config, conexion_memory_stick, conexion_kernel_memory);
    
    saludar("kernel_scheduler");
    return 0;
}
