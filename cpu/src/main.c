#include <utils/utils.h>


void get_config(t_log* logger, t_config* config, char* clave_ip, char* clave_puerto, char** ip, char** puerto)
{
    if(config_has_property(config,clave_ip)){
        *ip = config_get_string_value(config, clave_ip);
    }else{
        log_error(logger, "FALTA CLAVE: %s", clave_ip);
        exit(1);
    }
    if(config_has_property(config,clave_puerto)){
        *puerto = config_get_string_value(config, clave_puerto);
    }else{
        log_error(logger, "FALTA CLAVE: %s", clave_puerto);
        exit(1);
    } 
}


int main(int argc, char* argv[]) {
    t_log* logger = log_create("modulo_io.log", "modulo_io", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_scheduler;
    char* puerto_memory_stick;
    char* puerto_kernel_memory;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Identificador]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // CONEXION CLIENTE CON KERNEL SCHEDULER
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_config(logger, config, "IP", "PUERTO_KERNEL_SCHEDULER", &ip, &puerto_kernel_scheduler);
    
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
    log_info(logger, "> Modulo CPU Listo");
    // Conexion para scheduler
	int conexion = crear_conexion(ip, puerto_kernel_scheduler);

    // CONEXION CLIENTE CON MEMORY STICK
    get_config(logger, config, "IP", "PUERTO_MEMORY_STICK", &ip, &puerto_memory_stick);
    
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_MEMORY_STICK: %s", puerto_memory_stick);
    log_info(logger, "> Modulo CPU Listo");
    // Conexion para scheduler
	int conexion_stick = crear_conexion(ip, puerto_memory_stick);

    // CONEXION CLIENTE CON KERNEL MEMORY
    get_config(logger, config, "IP", "PUERTO_KERNEL_MEMORY", &ip, &puerto_kernel_memory);
    
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);
    log_info(logger, "> Modulo CPU Listo");
    // Conexion para scheduler
	int conexion_memory = crear_conexion(ip, puerto_kernel_memory);


    liberar_conexion(conexion);
    liberar_conexion(conexion_stick);
    liberar_conexion(conexion_memory);
    log_destroy(logger);
    config_destroy(config);

    
    saludar("cpu");
    return 0;
}

