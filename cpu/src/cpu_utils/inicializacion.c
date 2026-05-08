#include "inicializacion.h"

void validarArgumentos (int cantidadArgumentos){
    if (cantidadArgumentos != 3) {
        printf("La cantidad de argumentos ingresados no coincide. Formato -> [Archivo Config] [Identificador]\n");
        exit(EXIT_FAILURE) ;
    }
    return;
}

void inicializarModulo(char* pathConfig,char* identificador){
    // Instanciamos el config
    config = abrirConfig(pathConfig);
    
    //Leemos los valores y guardamos en las variables globales
    get_string_from_config(config, "LOG_LEVEL", &LOG_LEVEL);
    get_string_from_config(config, "IP_KERNEL_SCHEDULER", &IP_KERNEL_SCHEDULER);
    get_string_from_config(config, "IP_KERNEL_MEMORY", &IP_KERNEL_MEMORY);
    get_string_from_config(config, "IP_MEMORY_STICK", &IP_MEMORY_STICK);
    get_string_from_config(config, "PUERTO_KERNEL_MEMORY", &PUERTO_KERNEL_MEMORY);
    get_string_from_config(config, "PUERTO_KERNEL_SCHEDULER", &PUERTO_KERNEL_SCHEDULER);
    get_string_from_config(config, "PUERTO_MEMORY_STICK", &PUERTO_MEMORY_STICK);


    //Creamos el logger apartir del valor del config
    t_log_level nivelLogger = log_level_from_string(LOG_LEVEL);
    logger = log_create("cpu.log", "cpu", 1, nivelLogger);

    IDENTIFICADOR=(uint32_t) atoi(identificador); 
}