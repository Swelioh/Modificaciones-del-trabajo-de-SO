#include <utils/utils.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_swap)
{
    liberar_conexion(conexion_swap);
    log_destroy(logger);
    config_destroy(config);
}

int main(int argc, char* argv[]) {
    char* ip;
    char* puerto_kernel_memory;
    
    t_log* logger = log_create("swap.log", "swap", 1, LOG_LEVEL_TRACE);
    
    if (argc != 2){
        log_error(logger, "Uso: %s [Archivo Config] [Identificador]\n", argv[0]);
        //TODO: liberar logueo cada vez que falla
        return EXIT_FAILURE;
    }

    // Archivos de Config
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(logger, config, "IP", &ip);
    get_string_from_config(logger, config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);

    // Conexion a kernel memory
	int conexion = crear_conexion(ip, puerto_kernel_memory);
    log_info(logger, "> Swap Listo");

    liberar_recursos(logger, config, conexion);
    
    saludar("swap");
    return 0;
}
