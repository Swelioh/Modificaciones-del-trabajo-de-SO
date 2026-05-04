#include <utils_io/inicializacion.h>
#include <utils_io/administracionesTipo.h>

int main(int argc, char* argv[]) {
    // Validamos que haya 2 argumentos
    validarArgumentos (argc);

    // Cargamos el archivo de config y obtenemos el tipo de modulo que pusimos por parametro
    inicializarModulo(argv[1], argv[2]);

    // Logueamos info adicional
	log_debug(logger, "IP: %s", IP_KERNEL_SCHEDULER);
	log_debug(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_SCHEDULER);
    log_debug(logger, "> Modulo io Listo");

    // Nos conectamos con el Scheduler, en caso de error finaliza el programa
    int socket_kernel_scheduker = iniciarConexionKernelScheduler(logger);

    // Enviamos handshake al Scheduler para informarle el tipo de IO que somos
    enviarIngresoAScheduler(socket_kernel_scheduker);
    
    // En base al modulo que creamos administramos la comunicacion de forma diferente
    switch(tipo_modulo)
    {
        case SLEEP:
            administrarIoTipoSleep(socket_kernel_scheduker);
            break;
        case STDIN:
            administrarIoTipoStdin(socket_kernel_scheduker);
            break;
        case STDOUT:
            administrarIoTipoStdout(socket_kernel_scheduker);
            break;
        default: // El default no hace falta que tenga nada porque ya en este punto es si o si uno de esos 3
            break;
    }
    
    close(socket_kernel_scheduker);
    liberarModulo(logger, config);
    return 0;
}