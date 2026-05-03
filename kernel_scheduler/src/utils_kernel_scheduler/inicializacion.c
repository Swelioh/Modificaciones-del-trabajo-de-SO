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
    config = abrirConfig(pathConfig);

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

    //Inicializamos las estructuras y semaforos que necesitamos

    //Para CPU
    cpus_conectadas = list_create();
    pthread_mutex_init(&mutex_lista_cpus, NULL);

    //Para IO
    for(int i = 0; i < CANTIDAD_TOTAL_IO; i++) //Por cada hilo inicializo sus instancias de listas y semaforos
    {
        tareas_io_pendientes[i] = queue_create();
        pthread_mutex_init(&(mutex_lista_tareas_io[i]), NULL);
        sem_init(&semaforo_tareas_io_pendientes[i], 0, 0); // Los inicializo en 0 porque no hay tareas pendientes al principio
    }

    return;
}

// Me devuelve el fd del socket del kernel memory
int iniciarConexionKernelMemory(t_log* logger, char* ip_memory, char* puerto)
{
    // Nos conectamos al Kernel Memory
	int socket_memory = crear_conexion(logger, IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);
    if(socket_memory == -1)
    {
        log_error(logger, "No se pudo establecer la conexion con el Modulo Kernel Memory. Finalizando el programa.");
        liberarModulo(logger, config);
        exit(EXIT_FAILURE);
    }
    log_info(logger, "## Conectado a Kernel Memory"); //LOG_OBLIGATORIO
    return socket_memory;
}

// Me libera la memoria de todas las cosas que pedi dinamicamente
void liberarModulo() {
    seguir_ejecutando = 0; // TODO: No se si sirve salir del while del hilo que atiende las conexiones, me parece que es medio al pedo

    // Liberamos todas las estructuras que pedi de IO
    for(int i = 0; i < CANTIDAD_TOTAL_IO; i++) //Por cada hilo destruyo sus instancias de colas y semaforos
    {
        int tamanio_cola = queue_size(tareas_io_pendientes[i]);
        for (int j = 0; j < tamanio_cola; j ++)
        {
            t_tarea_io* tarea_a_eliminar = queue_pop(tareas_io_pendientes[i]); // Saco una tarea de la lista
            free(tarea_a_eliminar); // La elimino
        }
        queue_destroy(tareas_io_pendientes[i]); // Una vez que libere todos los elementos de la lista destruyo la cola
        pthread_mutex_destroy(&(mutex_lista_tareas_io[i]));
        sem_destroy(&semaforo_tareas_io_pendientes[i]); // Los inicializo en 0 porque no hay tareas pendientes al principio
    }
    // Cerramos el socket que teniamos como servidor
    close(socket_scheduler);
    // Destruimos el config y el logger
    log_destroy(logger);
    if(!string_array_is_empty(QUEUES_ALGORITHMS))
        string_array_destroy(QUEUES_ALGORITHMS);
    config_destroy(config);
}

void crear_proceso(char* path, int prioridad){
    printf("FALTA IMPLEMENTAR");
    // log_debug(logger,
    //     "Se solicita crear proceso -> PATH: %s | PRIORIDAD: %d",
    //     path,
    //     prioridad
    // );
}

void procesar_fin(int pid){
    printf("FALTA IMPLEMENTAR");
    // log_debug(logger,
    //     "Finalización de proceso -> PID: %d ",
    //     pid
    // );
}