#include <utils_kernel_scheduler/inicializacion.h>
#include <utils_kernel_scheduler/comunicacionProcesador.h>
#include <utils_kernel_scheduler/comunicacionIO.h>

int main(int argc, char* argv[]) {

    validarArgumentos (argc);

    inicializarModulo(argv[1]);

	log_info(logger, "IP: %s", IP_KERNEL_MEMORY);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Scheduler Listo");

    // int socket_kernel_memory = iniciarConexionKernelMemory(logger, IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);

    //Iniciamos servidor para escuchar conexiones de CPU e IO
	int socket_servidor = iniciar_servidor(logger, PUERTO_KERNEL_SCHEDULER);

    while (1) {
        // Esperamos a que se conecte un cliente
        int fd_cliente = esperar_cliente(socket_servidor, logger);

        // Obtengo el mensaje de handshake del modulo
        t_paquete* paquete = recibir_paquete_completo(fd_cliente);

        if(paquete == NULL){ // Si hubo un error en el handshake cierro la conexion
            close(fd_cliente);
            continue;
        }
        
        // Dependiendo que modulo sea creo un hilo para atenderlo
        switch (paquete->codigo_operacion) {
            case INGRESO_CPU:
                iniciarCpu(fd_cliente, paquete->buffer);
                break;
            case INGRESO_IO:
                iniciarIO(fd_cliente, paquete->buffer);
                break;
            default:
                log_warning(logger,"Operacion desconocida. No quieras meter la pata");
                break;
		}

        // Libero la memoria que se habia pedido para almacenar el paquete
        if(paquete != NULL)
            eliminar_paquete(paquete);
	}
    
    // Liberar recursos TODO!!! Cada Hilo maneja su desconexion.
    /*liberar_conexion(conexion);
    liberar_conexion(conexion_servidor);*/
    liberarModulo(logger, config);
    return 0;
}