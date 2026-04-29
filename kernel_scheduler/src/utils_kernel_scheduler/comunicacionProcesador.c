#include "comunicacionProcesador.h"

void iniciarCpu(int socket_cpu, t_buffer* buffer)
{
    // Creo la estructura donde deserializo el mensaje
    t_ingreso_cpu mensaje_ingreso;
    deserializarIngresoCPU(buffer, &mensaje_ingreso);

    // Creo la estructura que va a almacenar los argumentos que se pasan a la funcion del p_thread
    t_handler_cpu* args = malloc(sizeof(t_handler_cpu));
    args->socket_cpu = socket_cpu;
    args->info_cpu = mensaje_ingreso;

    // TODO: Yo no crearia una lista con el mismo tipo del mensaje de ingreso, conviene tener una estructura dedicada para administrar las CPUs disponibles
    pthread_mutex_lock(&mutex_lista_cpus);
    list_add(cpus_conectadas, args);
    pthread_mutex_unlock(&mutex_lista_cpus);
    sem_post(&cpu_disponible);

    pthread_t hilo;
    pthread_create(&hilo, NULL, handlerCPU, args);
    pthread_detach(hilo);
}

void* handlerCPU(void* argumentos){
    // Casteo el void* a la estructura que guarda los argumentos
    t_handler_cpu* args = (t_handler_cpu*) argumentos;

    // Me quedo con los datos que necesito y libero la memoria que habia pedido para almacenar los argumentos
    int socketCpu = args->socket_cpu;
    t_ingreso_cpu info_cpu = args->info_cpu;
    free(args);

    // TODO: Administrar la CPU
    log_info(logger, "## CPU %d Conectada", info_cpu.identificador_cpu); //LOG_OBLIGATORIO

    return NULL;
}