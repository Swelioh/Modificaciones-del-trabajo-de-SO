#include <utils/utils.h>

int main(int argc, char* argv[]) {
    t_log* logger = log_create("kernel_scheduler.log", "kernel_scheduler", 1, LOG_LEVEL_TRACE);

    log_info(logger, "> Kernel Scheduler Listo");
    
    // Creamos una conexión hacia el servidor
	int conexion = crear_conexion("127.0.0.1", "4444");


    // TODO: hacer un servidor multihilo para atender peticiones de IO y CPU.

    // char * leido;

    // leido = readline("> ");

    // free(leido);

    //Liberar recursos TODO!!!
    liberar_conexion(conexion);
    
    saludar("kernel_scheduler");
    return 0;
}
