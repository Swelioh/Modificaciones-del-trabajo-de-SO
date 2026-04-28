// #include <utils/inicializacion.h>
#include <serializacion/estructuras.h>
#include <utils/inicializacion.h>
#include <serializacion/serializacion.h>
#include <pthread.h>

typedef struct {
    int fd;
    int conexion_servidor;
} t_args;

t_paquete* recibir_paquete_completo(int socket) {

	//Reservamos memoria para el paquete a recibir
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));

    //Recibimos el codigo de operacion, validamos que no devuelva <= 0 porque indicaria un error en recv
    if (recv(socket, &(paquete->codigo_operacion), sizeof(uint8_t), 0) <= 0) {
        free(paquete->buffer);
        free(paquete);
        return NULL;
    }

    //Recibimos tamaño del buffer
    if (recv(socket, &(paquete->buffer->size), sizeof(uint32_t), 0) <= 0) {
        free(paquete->buffer);
        free(paquete);
        return NULL;
    }

    //Reservamos memoria para el payload
    paquete->buffer->stream = malloc(paquete->buffer->size);

    //Recibimos el payload completo
    if (recv(socket, paquete->buffer->stream, paquete->buffer->size, 0) <= 0) {
        free(paquete->buffer->stream);
        free(paquete->buffer);
        free(paquete);
        return NULL;
    }

    return paquete;
}

int main(int argc, char* argv[]) {
    //TODO hacer atexit(funcitions); para que se ejecuten cuando se usa exit(asdasd);
    validar_argumentos(argc);

    inicializar_modulo(argv[1]);

	// pthread_t hilo;

    log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Memory Listo");
	
    int conexion_servidor = iniciar_servidor(logger, PUERTO_KERNEL_MEMORY);

    // TODO: hacer multihilo
    // esperar_cliente(conexion_servidor, logger);
    // esperar_cliente(conexion_servidor, logger);
    // esperar_cliente(conexion_servidor, logger);
    // esperar_cliente(conexion_servidor, logger);

	printf("HOLAAA");
	while(1) {
		//Nos quedamos esperando a que se conecte algun modulo
		printf("Antes de esperar cliente");
		int fd_cliente = esperar_cliente(conexion_servidor, logger);
		printf("Despues de esperar cliente");

		//Handshake para saber quien se conecto
        t_paquete* hs = recibir_paquete_completo(fd_cliente);	// Se podria usar funcion: recibir_operacion que devuelve el cod_op nomas
        
		//Validamos que pudimos recibir el handshake, sino continuamos con la siguiente iteracion del bucle
		if (hs == NULL) { 
			close(fd_cliente); 
			continue; 
		}
		
		// Guardamos en una variable el cod_op para eliminar el paquete // TODO: revisar si el buffer lo vamos a usar para algo en esta instancia, porque por ahora solo con el cop_op ya podriamos identificar que cliente/modulo se contecto
        int tipo = hs->codigo_operacion;
		eliminar_paquete(hs);

		// Reservamos memoria para pasar ambos parámetros a los handlers
        t_args* args = malloc(sizeof(t_args));
        args->fd = fd_cliente;
        args->conexion_servidor = conexion_servidor;//TODO: hace falta este parametro???

		switch (tipo) {
			case 66666://HANDSHAKE_KERNEL_SCHEDULER
				log_info(logger, "Se conectó el kernel scheduler...");
				// pthread_create(&hilo, NULL, (void*) handler_kernel_scheduler, args);
				break;
			case HANDSHAKE_CPU:
				log_info(logger, "Se conectó el CPU...");
				// pthread_create(&hilo, NULL, (void*) handler_kernel_scheduler, args);
				break;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				//TODO: ver donde liberar recursos, capaz es aca
				break;
		}
	}

	//TODO
	// phthreadjoin(H1); // Hay que usar la otra


    // // RECIBIR PAQUETE
    // t_list* lista;
	// while (1) {
	// 	int cod_op = recibir_operacion(cliente_fd);
	// 	switch (cod_op) {
	// 	case PAQUETE:
	// 		lista = recibir_paquete(cliente_fd);
	// 		log_info(logger, "Me llegaron los siguientes valores:\n");
	// 		list_iterate(lista, (void*) iterator);
	// 		list_destroy_and_destroy_elements(lista, (void*) liberar_elemento);
	// 		break;
	// 	case -1:
	// 		log_error(logger, "el cliente se desconecto. Terminando servidor");
	// 		return EXIT_FAILURE;
	// 	default:
	// 		log_warning(logger,"Operacion desconocida. No quieras meter la pata");
	// 		break;
	// 	}
	// }

    liberar_recursos(logger, config);
    liberar_conexion(conexion_servidor);

    return 0;
}