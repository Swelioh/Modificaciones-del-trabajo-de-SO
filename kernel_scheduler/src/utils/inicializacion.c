#include "inicializacion.h"

void validarArgumentos (int cantidadArgumentos){
    if (cantidadArgumentos != 3) {
        printf("La cantidad de argumentos ingresados no coincide. Formato -> [Archivo Config] [Path Proceso Inicial]\n");
        exit(EXIT_FAILURE) ;
    }
    return;
}

void inicializarModulo(char* pathConfig){
    // Instanciamos el config
    config = config_create(pathConfig);

    //Leemos los valores y guardamos en las variables globales
    LOG_LEVEL = config_get_string_value(config, "LOG_LEVEL");
    PLANIFICATION_ALGORITHM = config_get_string_value(config, "PLANIFICATION_ALGORITHM");
    QUEUES_ALGORITHMS = config_get_array_value(config, "QUEUES_ALGORITHMS"); // TODO: HAY QUE LIBERAR ESTO
    RR_QUANTUM = 	config_get_int_value(config, "RR_QUANTUM");
    QUEUE_PREEMPTION = config_get_string_value(config, "QUEUE_PREEMPTION");
    SUSPENSION_TIMEOUT = 	config_get_int_value(config, "SUSPENSION_TIMEOUT"); 
    IP_KERNEL_MEMORY = config_get_string_value(config,"IP_KERNEL_MEMORY");
    PUERTO_KERNEL_MEMORY = config_get_string_value(config, "PUERTO_KERNEL_MEMORY");
    PUERTO_KERNEL_SCHEDULER = config_get_string_value(config, "PUERTO_KERNEL_SCHEDULER");

    //Creamos el logger apartir del valor del config
    t_log_level nivelLogger = log_level_from_string(LOG_LEVEL);
    logger = log_create("kernel_scheduler.log", "kernel_scheduler", 1, nivelLogger);
    return;
}

void liberarModulo(t_log* logger,t_config* config){
    log_destroy(logger);
    if(!string_array_is_empty(QUEUES_ALGORITHMS))
        string_array_destroy(QUEUES_ALGORITHMS);
    config_destroy(config);
}