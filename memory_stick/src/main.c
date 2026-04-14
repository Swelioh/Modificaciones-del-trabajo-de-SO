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
 
    t_log* logger = log_create("memory_stick.log", "memory_stick", 1, LOG_LEVEL_TRACE);

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
    get_config(logger, config, "IP", "PUERTO_KERNEL_MEMORY", &ip, &puerto_kernel_memory);
	log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);
    log_info(logger, "> Memory stick Listo");
    // Conexion para memory
	int conexion = crear_conexion(ip, puerto_kernel_memory);

    // SERVIDOR PARA CPU / IO / (Memory?)(Puerto distinto del server de memory)
    get_config(logger, config, "IP", "PUERTO_MEMORY_STICK", &ip, &puerto_servidor);

	int conexion_servidor = iniciar_servidor(logger, puerto_servidor);
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
