#include <utils/utils.h>

int main(int argc, char* argv[]) {

    t_log* logger = log_create("kernel_memory.log", "kernel_memory", 1, LOG_LEVEL_TRACE);

    log_info(logger, "> Kernel Memory Listo");

    int socket_servidor = iniciar_servidor(logger, "4444");//TODO pasar el puerto a config !!!

    esperar_cliente(socket_servidor, logger);
    esperar_cliente(socket_servidor, logger);
    esperar_cliente(socket_servidor, logger);
    esperar_cliente(socket_servidor, logger);

    //Liberar recursos TODO!!!
    liberar_conexion(socket_servidor);
    log_destroy(logger);

    saludar("kernel_memory");
    return 0;
}
