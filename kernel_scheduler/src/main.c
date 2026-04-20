#include <utils/inicializacion.h>

int main(int argc, char* argv[]) {

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
    // Esperamos a la IO
    esperar_cliente(conexion_servidor, logger);
    // Esperamos a la CPU
    esperar_cliente(conexion_servidor, logger);
    

    //Liberar recursos TODO!!!
    liberar_conexion(conexion);
    liberar_conexion(conexion_servidor);
    liberarModulo(logger, config);
    return 0;
}
