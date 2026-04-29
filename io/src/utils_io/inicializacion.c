#include "inicializacion.h"

// Funcion que comprueba que se haya ingresado la cantidad valida de argumentos a la hora de correr el modulo
void validarArgumentos (int cantidadArgumentos){
    if (cantidadArgumentos != 3) {
        printf("La cantidad de argumentos ingresados no coincide. Formato -> [Archivo Config] [Tipo]\n");
        exit(EXIT_FAILURE) ;
    }
    return;
}

// Funcion que obtiene del config las variables, inicializa el logger y recupera el tipo de modulo que se ingreso por parametro
void inicializarModulo(char* pathConfig, char* tipo){
    // Instanciamos el config
    config = abrirConfig(pathConfig);

    //Leemos los valores y guardamos en las variables globales
    get_string_from_config(config, "LOG_LEVEL", &LOG_LEVEL);
    get_string_from_config(config, "IP_KERNEL_SCHEDULER", &IP_KERNEL_SCHEDULER);
    get_string_from_config(config, "PUERTO_KERNEL_SCHEDULER", &PUERTO_KERNEL_SCHEDULER);

    //Creamos el logger apartir del valor del config
    t_log_level nivelLogger = log_level_from_string(LOG_LEVEL);
    logger = log_create("io.log", "io", 1, nivelLogger);

    //Guardamos el tipo que es este modulo
    tipo_modulo = tipo_io_string_a_enum(tipo);
    if(tipo_modulo == IO_INVALIDO){
        log_error(logger, "No se reconocio el tipo de IO. Vuelva a intentar.");
        liberarModulo(logger, config);
        exit(EXIT_FAILURE);
    }

    return;
}

// Inicia la conexion con el scheduler y me devuelve el fd para su comunicacion
int iniciarConexionKernelScheduler(t_log* logger)
{
    // Nos conectamos al Kernel Scheduler
	int socket_memory = crear_conexion(logger, IP_KERNEL_SCHEDULER, PUERTO_KERNEL_SCHEDULER);
    if(socket_memory == -1)
    {
        log_error(logger, "No se pudo establecer la conexion con el Modulo Kernel Scheduler. Finalizando el programa.");
        liberarModulo(logger, config);
        exit(EXIT_FAILURE);
    }
    log_info(logger,"## Conectado a Kernel Scheduler"); //LOG_OBLIGATORIO
    return socket_memory;
}

// Funcion que me convierte el string del tipo de io que es y me lo transforma al enum de tipos (DEVUELVE -1 SI NO SE RECONOCIO EL TIPO)
t_tipos_io tipo_io_string_a_enum(char* tipo)
{
    if (strcmp(tipo, "SLEEP") == 0) {
        return SLEEP;
    } else if (strcmp(tipo, "STDIN") == 0) {
        return STDIN;
    } else if (strcmp(tipo, "STDOUT") == 0) {
        return STDOUT;
    } else {
        return IO_INVALIDO;
    }
}

// Funcion que libera el config y logger que cree
void liberarModulo(t_log* logger_a_destruir,t_config* config_a_destruir){
    log_destroy(logger_a_destruir);
    config_destroy(config_a_destruir);
}

// Funcion de handshake para enviarle al scheduler que tipo de io soy
void enviarIngresoAScheduler(int socket_scheduler)
{
    // Creo el mensaje
    t_ingreso_io mensaje_ingreso;
    mensaje_ingreso.tipo_io = tipo_modulo;
    // Armo paquete y envio
    t_paquete* paquete = armar_paquete(INGRESO_IO, &mensaje_ingreso);
    enviar_paquete(paquete, socket_scheduler);
	eliminar_paquete(paquete);
    return;
}