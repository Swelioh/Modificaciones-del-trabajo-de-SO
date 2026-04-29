#include <serializacion/estructuras.h>
#include <utils_memory/inicializacion.h>
#include <variables_globales/variables_globales.h>
#include <serializacion/serializacion.h>
#include <handlers_kernel_memory/handler_cpu.h>
#include <handlers_kernel_memory/handler_kernel_scheduler.h>
#include <pthread.h>
// TODO: armar un archivo .h para centralizar los includes

int main(int argc, char* argv[]) {

    //TODO hacer atexit(funcitions); para que se ejecuten cuando se usa exit(asdasd);
    validar_argumentos(argc);

    inicializar_modulo(argv[1]);

    log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Memory Listo");
	
    int conexion_servidor = iniciar_servidor(logger, PUERTO_KERNEL_MEMORY);

	while(1) {
    	pthread_t hilo;

		//Nos quedamos esperando a que se conecte algun modulo
		int fd_cliente = esperar_cliente(conexion_servidor, logger);

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

		int hilo_creado = 0;

		switch (tipo) {
			case HANDSHAKE_KERNEL_SCHEDULER:
				log_info(logger, "Se conectó el kernel scheduler...");
				pthread_create(&hilo, NULL, handler_kernel_scheduler, args);
				hilo_creado = 1;
				break;
			case HANDSHAKE_CPU:
				log_info(logger, "Se conectó el CPU...");
				pthread_create(&hilo, NULL, handler_cpu, args);
				hilo_creado = 1;
				break;
            case HANDSHAKE_SWAP:
				log_info(logger, "Se conectó el swap...");
				// pthread_create(&hilo, NULL, (void*) handler_swap, args);
				// hilo_creado = 1;
				break;
			case HANDSHAKE_MEMORY_STICK:
				log_info(logger, "Se conectó el memory stick...");
				// pthread_create(&hilo, NULL, (void*) handler_memory_stick, args);
				// hilo_creado = 1;
				break;
			default:
				log_warning(logger,"Operacion desconocida. No quieras meter la pata");
				close(fd_cliente);
				break;
		}

		if(hilo_creado)
			pthread_detach(hilo);
	}

	//TODO
	// phthreadjoin(H1); // Hay que usar la otra

    liberar_recursos(logger, config);
    liberar_conexion(conexion_servidor);

    return 0;
}