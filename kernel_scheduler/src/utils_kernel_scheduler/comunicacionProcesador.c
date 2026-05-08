#include "comunicacionProcesador.h"


void* handlerCPU(void* argumentos) {
    t_args_handler_cpu* args = (t_args_handler_cpu*) argumentos;
    int socket_cpu           = args->socket_cpu;
    t_buffer* buffer         = args->buffer;
    free(args);

    // Deserializo el handshake con el buffer que ya vino del main
    t_ingreso_cpu info_cpu;
    deserializarIngresoCPU(buffer, &info_cpu);

    log_info(logger, "## CPU %d Conectada", info_cpu.identificador_cpu); //LOG_OBLIGATORIO

    /*pthread_mutex_lock(&mutex_lista_cpus);
    list_add(cpus_conectadas, info_cpu);
    pthread_mutex_unlock(&mutex_lista_cpus);*/

    sem_post(&cpu_disponible);

    // Ciclo principal: atender syscalls de esta CPU
    while(1) {
        t_paquete* paquete = recibir_paquete_completo(socket_cpu);
        if(paquete == NULL) {
            // La CPU se desconecto
            log_debug(logger, "CPU %d desconectada.", info_cpu.identificador_cpu);
            close(socket_cpu);
            return NULL;
        }

        switch(paquete->codigo_operacion) {
            case OP_CREAR_PROCESO: {
                //t_syscall_crear_proceso syscall;
                //deserializarSyscallCrearProceso(paquete->buffer, &syscall);
                //log_info(logger, "## (%d) - Solicitó syscall: CREAR_PROCESO", pid_proceso); //LOG_OBLIGATORIO
                // El proceso pasa de EXEC a BLOCK
                // TODO: moverlo a la cola de BLOCK y enviarselo al modulo IO correspondiente
                break;
            }
            case OP_FIN_PROCESO: {
                //t_syscall_fin_proceso syscall;
                //deserializarSyscallFinProceso(paquete->buffer, &syscall);
                //log_info(logger, "## (%d) - Solicitó syscall: FIN_PROCESO", pid_proceso); //LOG_OBLIGATORIO
                // El proceso pasa de EXEC a BLOCK
                // TODO: moverlo a la cola de BLOCK y enviarselo al modulo IO de tipo STDIN
                break;
            }
            case OP_SYSCALL: {
                //t_syscall_io_stdout syscall;
                //deserializarSyscallIoStdout(paquete->buffer, &syscall);
                //log_info(logger, "## (%d) - Solicitó syscall: STDOUT", pid_proceso); //LOG_OBLIGATORIO
                // El proceso pasa de EXEC a BLOCK
                // TODO: pedirle los bytes al Kernel Memory y enviarselos al modulo IO de tipo STDOUT
                break;
            }
            case OP_INTERRUPCION: {
                //t_syscall_interrupcion syscall;
                //deserializarSyscallInterrupcion(paquete->buffer, &syscall);
                //log_info(logger, "## (%d) - Solicitó syscall: INTERRUPCION", pid_proceso); //LOG_OBLIGATORIO
                // TODO: crear el mutex si no existe
                break;
            }
            default:
                log_warning(logger, "Codigo de operacion desconocido recibido de CPU %d", info_cpu.identificador_cpu);
                break;
        }

        eliminar_paquete(paquete);
    }

    return NULL;
}