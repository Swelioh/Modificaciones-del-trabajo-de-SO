#include <utils/utils.h>

void liberar_recursos(t_log* logger, int conexion_scheduler)
{
    liberar_conexion(conexion_scheduler);
    log_destroy(logger);
}

int main(int argc, char* argv[]) {
    char* puerto_kernel_memory;

    t_log* logger = log_create("kernel_memory.log", "kernel_memory", 1, LOG_LEVEL_TRACE);

    log_info(logger, "> Kernel Memory Listo");

    // Archivos de Config
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(logger, config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);
	
    log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);
    
    int socket_servidor = iniciar_servidor(logger, puerto_kernel_memory);

    esperar_cliente(socket_servidor, logger);
    esperar_cliente(socket_servidor, logger);
    esperar_cliente(socket_servidor, logger);
    esperar_cliente(socket_servidor, logger);

    liberar_recursos(logger, socket_servidor);

    saludar("kernel_memory");
    return 0;
}
