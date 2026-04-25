#include <utils/inicializacion.h>

// TODO: REVISARRRRRR
void iterator(char* value) {
	log_info(logger,"%s", value);
}

int main(int argc, char* argv[]) {
    //TODO hacer atexit(funcitions); para que se ejecuten cuando se usa exit(asdasd);
    validar_argumentos(argc);

    inicializar_modulo(argv[1]);

    log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Memory Listo");
	
    int conexion_servidor = iniciar_servidor(logger, PUERTO_KERNEL_MEMORY);

    // TODO: hacer multihilo
    esperar_cliente(conexion_servidor, logger);
    esperar_cliente(conexion_servidor, logger);
    esperar_cliente(conexion_servidor, logger);
    esperar_cliente(conexion_servidor, logger);

    // RECIBIR PAQUETE
    t_list* lista;
	while (1) {
		int cod_op = recibir_operacion(cliente_fd);
		switch (cod_op) {
		case PAQUETE:
			lista = recibir_paquete(cliente_fd);
			log_info(logger, "Me llegaron los siguientes valores:\n");
			list_iterate(lista, (void*) iterator);
			list_destroy_and_destroy_elements(lista, (void*) liberar_elemento);
			break;
		case -1:
			log_error(logger, "el cliente se desconecto. Terminando servidor");
			return EXIT_FAILURE;
		default:
			log_warning(logger,"Operacion desconocida. No quieras meter la pata");
			break;
		}
	}

    liberar_recursos(logger, config);
    liberar_conexion(conexion_servidor);

    return 0;
}