#include <utils/utils.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler, int conexion_memory)
{
    liberar_conexion(conexion_scheduler);
    liberar_conexion(conexion_memory);
    log_destroy(logger);
    config_destroy(config);
}

int main(int argc, char* argv[]) {
    t_log* logger = log_create("kernel_scheduler.log", "kernel_scheduler", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_memory;
    char* puerto_kernel_scheduler;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Path Proceso Inicial]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // CONEXION CLIENTE CON KERNEL MEMORY
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(logger, config, "IP", &ip);
    get_string_from_config(logger, config, "PUERTO_KERNEL_SCHEDULER", &puerto_kernel_scheduler);
    get_string_from_config(logger, config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);
	
    log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);
    log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
    
    // Conexion para memory
	int conexion_kernel_memory = crear_conexion(ip, puerto_kernel_memory);
    log_info(logger, "> Kernel Scheduler Conectado a Kernel Memory");

    // TODO: Hacer servidor multihilo
    // SERVIDOR PARA CPU / IO
	int conexion_scheduler = iniciar_servidor(logger, puerto_kernel_scheduler);
   
    // Esperamos a la IO
    esperar_cliente(conexion_scheduler, logger);
    // Esperamos a la CPU
    esperar_cliente(conexion_scheduler, logger);

    liberar_recursos(logger, config, conexion_scheduler, conexion_kernel_memory);

    saludar("kernel_scheduler");
    return 0;
}
