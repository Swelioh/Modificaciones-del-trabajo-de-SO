#include <utils/inicializacion.h>

int main(int argc, char* argv[]) {
    pthread_t hilo;

    validarArgumentos (argc);

    inicializarModulo(argv[1]);

	log_info(logger, "IP: %s", IP_KERNEL_MEMORY);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Scheduler Listo");

    // Conexion para memory
	int conexion = crear_conexion(IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);

    // TODO: Hacer servidor multihilo
    // SERVIDOR PARA CPU / IO / (Memory?)(Puerto distinto del server de memory)
	int conexion_servidor = iniciar_servidor(logger, PUERTO_KERNEL_SCHEDULER);
    
    int* fd_ptr = malloc(sizeof(int));
    *fd_ptr = conexion_servidor;

    pthread_create(&hilo, NULL, hilo_aceptador, fd_ptr);
    
    //Liberar recursos TODO!!! Cada Hilo maneja su desconexion.
    /*liberar_conexion(conexion);
    liberar_conexion(conexion_servidor);
    liberarModulo(logger, config);*/
    return 0;
}
