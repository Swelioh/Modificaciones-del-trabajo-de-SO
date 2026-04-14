#include <utils/utils.h>

int main(int argc, char* argv[]) {
 
    t_log* logger = log_create("kernel_scheduler.log", "kernel_scheduler", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Path Proceso Inicial]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Config TODO completo 
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

    if(config_has_property(config,"PUERTO")){
        puerto = config_get_string_value(config, "PUERTO");
    }else{
        log_error(logger, "Falta PUERTO");
        return EXIT_FAILURE;
    } 

	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO: %s", puerto);

    log_info(logger, "> Kernel Scheduler Listo");
    
    // Creamos una conexión hacia el servidor
	int conexion = crear_conexion(ip, puerto);

    //Aca se levantaria un server con un puerto distinto al del server memory

    //Liberar recursos TODO!!!
    liberar_conexion(conexion);
    

    saludar("kernel_scheduler");
    return 0;
}
