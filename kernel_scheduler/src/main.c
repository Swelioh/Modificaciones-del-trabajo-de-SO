#include <utils/inicializacion.h>

int main(int argc, char* argv[]) {
    // pthread_t hilo;

    validarArgumentos (argc);

    inicializarModulo(argv[1]);

	log_info(logger, "IP: %s", IP_KERNEL_MEMORY);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Scheduler Listo");

    // Nos conectamos al Kernel Memory
	int conexion_memory = crear_conexion(logger, IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);
    if(conexion_memory == -1)
    {
        log_error(logger, "No se pudo establecer la conexion con el Modulo Kernel Memory. Finalizando el programa.");
        liberarModulo(logger, config);
        exit(EXIT_FAILURE);
    }

    // //Iniciamos servidor para escuchar conexiones de CPU e IO
	// int conexion_servidor = iniciar_servidor(logger, PUERTO_KERNEL_SCHEDULER);

    // while (1) {
    //     int fd_cliente = esperar_cliente(conexion_servidor, logger);

    //     // Reservamos memoria para pasar ambos parámetros
    //     t_args* args = malloc(sizeof(t_args));
    //     args->fd = fd_cliente;
    //     args->conexion_servidor = conexion_servidor;

    //     // handshake. Crear un estructura para regitrar lo que tenemos
    //     t_paquete* hs = recibir_paquete_completo(fd);
    //     if (hs == NULL) { close(fd); return NULL; }
    //     int tipo = hs->codigo_operacion;
    //     eliminar_paquete(hs);

    //     switch (hs) {
	// 	case HANDSHAKE_CPU:
	// 		    pthread_create(&hilo, NULL, (void*) handler_cpu, args);
	// 		break;
	// 	case HANDSHAKE_IO:
	// 		    pthread_create(&hilo, NULL, (void*) handler_io, args);
	// 		break;
	// 	case -1:
	// 		log_error(logger, "el cliente se desconecto. Terminando servidor");
	// 		return EXIT_FAILURE;
	// 	default:
	// 		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
	// 		break;
	// 	}
	// }
    
    //Liberar recursos TODO!!! Cada Hilo maneja su desconexion.
    /*liberar_conexion(conexion);
    liberar_conexion(conexion_servidor);*/
    liberarModulo(logger, config);
    return 0;
}
