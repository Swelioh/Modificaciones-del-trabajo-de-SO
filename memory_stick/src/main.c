#include <utils/utils.h>

int main(int argc, char* argv[]) {
    t_log* logger = log_create("memory_stick.log", "memory_stick", 1, LOG_LEVEL_TRACE);

    log_info(logger, "> Memory Stick Listo");
    
    // Creamos una conexión hacia el servidor
	int conexion = crear_conexion("127.0.0.1", "4444");

    // char * leido;

    // leido = readline("> ");

    // free(leido);

    //Liberar recursos TODO!!!
    liberar_conexion(conexion);


    saludar("memory_stick");
    return 0;
}
