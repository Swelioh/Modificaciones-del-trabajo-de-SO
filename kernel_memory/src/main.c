#include <utils/inicializacion.h>

int main(int argc, char* argv[]) {
    //TODO hacer atexit(funcitions); para que se ejecuten cuando se usa exit(asdasd);

    validar_argumentos(argc);

    inicializar_modulo(argv[1]);

    log_info(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Kernel Memory Listo");
	
    int conexion_servidor = iniciar_servidor(logger, PUERTO_KERNEL_MEMORY);

    esperar_cliente(conexion_servidor, logger);
    esperar_cliente(conexion_servidor, logger);
    esperar_cliente(conexion_servidor, logger);
    esperar_cliente(conexion_servidor, logger);

    liberar_recursos(logger, config);
    liberar_conexion(conexion_servidor);

    return 0;
}
