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
    if(config == NULL){
        printf("No se pudo inicializar el archivo de config, por favor revise el path. Actual: '%s'\n", pathConfig);
        exit(EXIT_FAILURE) ;
    }

    //Leemos los valores y guardamos en las variables globales
    get_string_from_config(config, "LOG_LEVEL", &LOG_LEVEL);
    get_string_from_config(config, "PLANIFICATION_ALGORITHM", &PLANIFICATION_ALGORITHM);
    get_array_from_config(config, "QUEUES_ALGORITHMS", &QUEUES_ALGORITHMS);
    get_int_from_config(config, "RR_QUANTUM", &RR_QUANTUM);
    get_string_from_config(config, "QUEUE_PREEMPTION", &QUEUE_PREEMPTION);
    get_int_from_config(config, "SUSPENSION_TIMEOUT", &SUSPENSION_TIMEOUT);
    get_string_from_config(config, "IP_KERNEL_MEMORY", &IP_KERNEL_MEMORY);
    get_string_from_config(config, "PUERTO_KERNEL_MEMORY", &PUERTO_KERNEL_MEMORY);
    get_string_from_config(config, "PUERTO_KERNEL_SCHEDULER", &PUERTO_KERNEL_SCHEDULER);

    //Creamos el logger apartir del valor del config
    t_log_level nivelLogger = log_level_from_string(LOG_LEVEL);
    logger = log_create("kernel_scheduler.log", "kernel_scheduler", 1, nivelLogger);
    return;
}

void liberarModulo(t_log* logger_a_destruir,t_config* config_a_destruir){
    log_destroy(logger_a_destruir);
    if(!string_array_is_empty(QUEUES_ALGORITHMS))
        string_array_destroy(QUEUES_ALGORITHMS);
    config_destroy(config_a_destruir);
}