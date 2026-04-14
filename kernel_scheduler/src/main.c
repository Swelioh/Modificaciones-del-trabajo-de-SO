#include <utils/utils.h>

int main(int argc, char* argv[]) {
 
    t_log* logger = log_create("kernel_scheduler.log", "kernel_scheduler", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_memory;
    char* puerto_servidor;

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
    if(config_has_property(config,"IP")){
        ip = config_get_string_value(config, "IP");
    }else{
        log_error(logger, "Falta IP");
        return EXIT_FAILURE;
    }
    if(config_has_property(config,"PUERTO_KERNEL_MEMORY")){
        puerto_kernel_memory = config_get_string_value(config, "PUERTO_KERNEL_MEMORY");
    }else{
        log_error(logger, "Falta PUERTO_KERNEL_MEMORY");
        return EXIT_FAILURE;
    } 
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);
    log_info(logger, "> Kernel Scheduler Listo");
    // Conexion para memory
	int conexion = crear_conexion(ip, puerto_kernel_memory);

    // TODO: Hacer servidor multihilo
    // SERVIDOR PARA CPU / IO / (Memory?)(Puerto distinto del server de memory)
    if(config_has_property(config,"PUERTO_SERVIDOR")){
        puerto_servidor = config_get_string_value(config, "PUERTO_SERVIDOR");
    }else{
        log_error(logger, "Falta PUERTO_SERVIDOR");
        return EXIT_FAILURE;
    } 

	int conexion_servidor = iniciar_servidor(logger, puerto_servidor);
    // Esperamos a la IO
    esperar_cliente(conexion_servidor, logger);
    // Esperamos a la CPU
    esperar_cliente(conexion_servidor, logger);
    

    //Liberar recursos TODO!!!
    liberar_conexion(conexion);
    liberar_conexion(conexion_servidor);
    log_destroy(logger);
    config_destroy(config);
    saludar("kernel_scheduler");
    return 0;
}
