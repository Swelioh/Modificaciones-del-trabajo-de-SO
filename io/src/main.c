#include <utils/utils.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler)
{
    liberar_conexion(conexion_scheduler);
    log_destroy(logger);
    config_destroy(config);
}

int main(int argc, char* argv[]) {
    t_log* logger = log_create("modulo_io.log", "modulo_io", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_scheduler;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Tipo]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Archivo de config
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(logger, config, "IP", &ip);
    get_string_from_config(logger, config, "PUERTO_KERNEL_SCHEDULER", &puerto_kernel_scheduler);

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
    
    // CONEXION CLIENTE CON KERNEL SCHEDULER
	int conexion_scheduler = crear_conexion(ip, puerto_kernel_scheduler);
    log_info(logger, "> Modulo IO Conectado a Scheduler");

    liberar_recursos(logger, config, conexion_scheduler);

    saludar("io");
    return 0;
}
