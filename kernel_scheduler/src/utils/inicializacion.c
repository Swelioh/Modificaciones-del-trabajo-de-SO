#include "inicializacion.h"
typedef struct {
    int fd;
    int conexion_servidor;
} t_args;

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

void* hilo_aceptador(void* arg) {
    int conexion_servidor =  *((int*) arg);  

    while (1) {
        int fd_cliente = esperar_cliente(conexion_servidor, logger);

        int* fd_cliente_ptr = malloc(sizeof(int));
        *fd_cliente_ptr = fd_cliente;

        // Reservamos memoria para pasar ambos parámetros
        t_args* args = malloc(sizeof(t_args));
        args->fd = fd_cliente;
        args->conexion_servidor = conexion_servidor;

        pthread_t hilo;
        pthread_create(&hilo, NULL, hilo_handler, args);
        pthread_detach(hilo);
    }
    return NULL;
}

void* hilo_handler(void* arg) {
    t_args* args = (t_args*) arg;
    int fd = args->fd;
    int conexion_servidor = args->conexion_servidor;
    free(args);

    // handshake
    t_paquete* hs = recibir_paquete_completo(fd);
    if (hs == NULL) { close(fd); return NULL; }
    int tipo = hs->codigo_operacion;
    eliminar_paquete(hs);

    while (1) {
        t_paquete* paquete = recibir_paquete_completo(fd);
        if (paquete == NULL) {
            manejar_desconexion(fd, tipo);
            close(fd);
            break;
        }

        // resetear offset para leer desde el principio
        paquete->buffer->offset = 0;

        switch (paquete->codigo_operacion) {

            case OP_CREAR_PROCESO: {
                char*   path      = leer_string_del_buffer(paquete->buffer);
                int32_t prioridad = leer_int32_del_buffer(paquete->buffer);
                crear_proceso(path, prioridad);

                free(path);
                break;
            }


            case OP_FIN_PROCESO: {
                int32_t pid = leer_int32_del_buffer(paquete->buffer);
                procesar_fin(pid);
                break;
            }

        }

        eliminar_paquete(paquete);
    }

    return NULL;
}

void manejar_desconexion(int conexion_servidor, int tipo_cliente){
    switch (tipo_cliente) {
        case TIPO_CPU:
            liberar_conexion(conexion_servidor);//desconectar_cpu(fd) Cada desconexion se resuelve de distinta forma.
            break;
        case TIPO_IO:
            liberar_conexion(conexion_servidor);//desconectar_io(fd); Cada desconexion se resuelve de distinta forma.
            break;
    }
}

void liberarModulo(t_log* logger_a_destruir,t_config* config_a_destruir){
    log_destroy(logger_a_destruir);
    if(!string_array_is_empty(QUEUES_ALGORITHMS))
        string_array_destroy(QUEUES_ALGORITHMS);
    config_destroy(config_a_destruir);
}

void crear_proceso(char* path, int prioridad){
    log_debug(logger,
        "Se solicita crear proceso -> PATH: %s | PRIORIDAD: %d",
        path,
        prioridad
    );
}

void procesar_fin(int pid){
    log_debug(logger,
        "Finalización de proceso -> PID: %d ",
        pid
    );
}