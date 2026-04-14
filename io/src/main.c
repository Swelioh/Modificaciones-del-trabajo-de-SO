#include <utils/utils.h>

int main(int argc, char* argv[]) {
    t_log* logger = log_create("modulo_io.log", "modulo_io", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_scheduler;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Tipo]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // CONEXION CLIENTE CON KERNEL SCHEDULER
     t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    if(config_has_property(config,"IP")){
        ip = config_get_string_value(config, "IP");
    }else{
        log_error(logger, "Falta IP");
        return EXIT_FAILURE;
    }
    if(config_has_property(config,"PUERTO_KERNEL_SCHEDULER")){
        puerto_kernel_scheduler = config_get_string_value(config, "PUERTO_KERNEL_SCHEDULER");
    }else{
        log_error(logger, "Falta PUERTO_KERNEL_SCHEDULER");
        return EXIT_FAILURE;
    } 
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
    log_info(logger, "> Modulo IO Listo");
    // Conexion para scheduler
	int conexion = crear_conexion(ip, puerto_kernel_scheduler);

    log_destroy(logger);
    config_destroy(config);
    liberar_conexion(conexion);

    saludar("io");
    return 0;
}
