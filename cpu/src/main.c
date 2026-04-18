#include <utils/utils.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler, int conexion_stick, int conexion_memory)
{
    liberar_conexion(conexion_scheduler);
    liberar_conexion(conexion_stick);
    liberar_conexion(conexion_memory);
    log_destroy(logger);
    config_destroy(config);
}

int main(int argc, char* argv[]) {
    t_log* logger = log_create("modulo_io.log", "modulo_io", 1, LOG_LEVEL_TRACE);

    char* ip;
    char* puerto_kernel_scheduler;
    char* puerto_memory_stick;
    char* puerto_kernel_memory;

    if (argc != 3) {
        log_error(logger, "Uso: %s [Archivo Config] [Identificador]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Archivos de Config
    t_config* config = config_create(argv[1]);
    if (config == NULL) {
        log_error(logger, "No se pudo cargar el config: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    get_string_from_config(logger, config, "IP", &ip);
    get_string_from_config(logger, config, "PUERTO_KERNEL_SCHEDULER", &puerto_kernel_scheduler);
    get_string_from_config(logger, config, "PUERTO_MEMORY_STICK", &puerto_memory_stick);
    get_string_from_config(logger, config, "PUERTO_KERNEL_MEMORY", &puerto_kernel_memory);

    log_info(logger, "IP: %s", ip);
	log_info(logger, "PUERTO_KERNEL_SCHEDULER: %s", puerto_kernel_scheduler);
	log_info(logger, "PUERTO_MEMORY_STICK: %s", puerto_memory_stick);
	log_info(logger, "PUERTO_KERNEL_MEMORY: %s", puerto_kernel_memory);

    // CONEXION CLIENTE CON KERNEL SCHEDULER
	int conexion_scheduler = crear_conexion(ip, puerto_kernel_scheduler);
    log_info(logger, "> Modulo CPU Conectado a Scheduler");

    // CONEXION CLIENTE CON MEMORY STICK
    int conexion_stick = crear_conexion(ip, puerto_memory_stick);
    log_info(logger, "> Modulo CPU Conectado a Memory Stick");

    // CONEXION CLIENTE CON KERNEL MEMORY
	int conexion_memory = crear_conexion(ip, puerto_kernel_memory);
    log_info(logger, "> Modulo CPU Conectado a Kernel Memory");

    liberar_recursos(logger, config, conexion_scheduler, conexion_stick, conexion_memory);

    saludar("cpu");
    return 0;
}
