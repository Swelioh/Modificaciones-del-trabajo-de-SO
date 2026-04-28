#include "inicializacion.h"

// typedef struct {
//     int fd;
//     int conexion_servidor;
// } t_args;

void validar_argumentos (int cantidadArgumentos){
    if (cantidadArgumentos != 2) {
        printf("La cantidad de argumentos ingresados no coincide. Formato -> [Archivo Config]\n");
        exit(EXIT_FAILURE) ;
    }
    return;
}

void inicializar_modulo(char* pathConfig){
    // Instanciamos el config
    config = config_create(pathConfig);

    if (config == NULL) {
        printf("No se pudo cargar el config: %s\n", pathConfig);
        exit(EXIT_FAILURE);
    }

    //Leemos los valores y guardamos en las variables globales
    get_string_from_config(config, "LOG_LEVEL", &LOG_LEVEL);
    get_int_from_config(config, "SEGMENT_MAX_SIZE", &SEGMENT_MAX_SIZE);
    get_string_from_config(config, "ALLOCATION_STRATEGY", &ALLOCATION_STRATEGY);
    get_int_from_config(config, "INSTRUCTION_DELAY", &INSTRUCTION_DELAY);
    get_int_from_config(config, "COMPACTION_DELAY", &COMPACTION_DELAY);
    get_string_from_config(config, "SCRIPTS_BASEPATH", &SCRIPTS_BASEPATH);
    get_string_from_config(config, "PUERTO_KERNEL_MEMORY", &PUERTO_KERNEL_MEMORY);

    //Creamos el logger apartir del valor del config
    t_log_level nivelLogger = log_level_from_string(LOG_LEVEL);
    logger = log_create("kernel_memory.log", "kernel_memory", 1, nivelLogger);

    return;
}

void liberar_recursos(t_log* logger_kernel_memory, t_config* config_kernel_memory){
    log_destroy(logger_kernel_memory);
    config_destroy(config_kernel_memory);
}