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
    t_config* config = abrirConfig(pathConfig);

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

void handler_cpu(t_args* args) {
    printf("FALTA IMPLEMENTAR");
    // int fd = args->fd;
    // free(args);

    // while (1) {
    //     t_paquete* paquete = recibir_paquete_completo(fd);

    //     if (paquete == NULL) {
    //         manejar_desconexion(fd, CPU);
    //         close(fd);
    //         break;
    //     }

    //     switch (paquete->codigo_operacion) {

    //         case OP_CREAR_PROCESO: {
    //             int32_t pid = leer_int32_del_buffer(paquete->buffer);
    //             string psuedocodigo = leer_string_del_buffer(paquete->buffer);
    //             procesar_fin(pid);
    //             break;
    //         }
            
    //         case OP_FIN_PROCESO: {
    //             int32_t pid = leer_int32_del_buffer(paquete->buffer);
    //             procesar_fin(pid);
    //             break;
    //         }

    //         case OP_SYSCALL: {
    //             int32_t syscall = leer_int32_del_buffer(paquete->buffer);
    //             manejar_syscall(syscall, paquete->buffer);
    //             break;
    //         }

    //         case OP_INTERRUPCION: {
    //             manejar_interrupcion();
    //             break;
    //         }
    //     }

    //     eliminar_paquete(paquete);
    // }
}

void handler_io(t_args* args) {
    printf("FALTA IMPLEMENTAR");
    // int fd = args->fd;
    // free(args);

    // while (1) {
    //     t_paquete* paquete = recibir_paquete_completo(fd);

    //     if (paquete == NULL) {
    //         manejar_desconexion(fd, IO);
    //         close(fd);
    //         break;
    //     }

    //     switch (paquete->codigo_operacion) {

    //         case OP_IO_FIN: {
    //             int32_t pid = leer_int32_del_buffer(paquete->buffer);
    //             finalizar_io(pid);
    //             break;
    //         }

    //     }

    //     eliminar_paquete(paquete);
    // }
}

void manejar_desconexion(int conexion_servidor, int tipo_cliente){
    printf("FALTA IMPLEMENTAR");
    // switch (tipo_cliente) {
    //     case TIPO_CPU:
    //         liberar_conexion(conexion_servidor);//desconectar_cpu(fd) Cada desconexion se resuelve de distinta forma.
    //         break;
    //     case TIPO_IO:
    //         liberar_conexion(conexion_servidor);//desconectar_io(fd); Cada desconexion se resuelve de distinta forma.
    //         break;
    // }
}

void liberarModulo(t_log* logger_a_destruir,t_config* config_a_destruir){
    log_destroy(logger_a_destruir);
    if(!string_array_is_empty(QUEUES_ALGORITHMS))
        string_array_destroy(QUEUES_ALGORITHMS);
    config_destroy(config_a_destruir);
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