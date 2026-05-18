#include <cpu_utils/cpu_utils.h>
#include <cpu_utils/inicializacion.h>
#include <cpu_utils/variablesGlobales.h>
#include <cpu_utils/instrucciones.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler, int conexion_stick, int conexion_memory)
{
    liberar_conexion(conexion_scheduler);
    liberar_conexion(conexion_stick);
    liberar_conexion(conexion_memory);
    liberar_cpu();
}



int main(int argc, char* argv[]) {

    validarArgumentos(argc);
    inicializarModulo(argv[1],argv[2]);


    log_debug(logger, "IP KERNEL SCHEDULER: %s", IP_KERNEL_SCHEDULER);
    log_debug(logger, "IP MEMORY STICK: %s", IP_MEMORY_STICK);
    log_debug(logger, "IP KERNEL MEMORY: %s", IP_KERNEL_MEMORY);
	log_debug(logger, "PUERTO_KERNEL_SCHEDULER: %s", PUERTO_KERNEL_SCHEDULER);
	log_debug(logger, "PUERTO_MEMORY_STICK: %s", PUERTO_MEMORY_STICK);
	log_debug(logger, "PUERTO_KERNEL_MEMORY: %s", PUERTO_KERNEL_MEMORY);

    // CONEXION CLIENTE CON KERNEL SCHEDULER
	int conexion_scheduler = crear_conexion(logger, IP_KERNEL_SCHEDULER, PUERTO_KERNEL_SCHEDULER);
    log_debug(logger, "> Modulo CPU Conectado a Scheduler");

    // =========================================================
    // 🤝 HANDSHAKE CON EL SCHEDULER
    // =========================================================
   // Extraemos el ID del parámetro
    uint32_t id_cpu = (uint32_t) atoi(argv[2]);
    t_ingreso_cpu handshake_cpu;
    handshake_cpu.identificador_cpu = id_cpu;

    // Armamdo y enviado del paquete 
    t_paquete* paquete_ingreso = armar_paquete_scheduler_cpu(HANDSHAKE_CPU, &handshake_cpu);
    enviar_paquete(paquete_ingreso, conexion_scheduler);
    eliminar_paquete(paquete_ingreso);

    log_info(logger, "Handshake enviado a Kernel Scheduler (Soy CPU ID: %d)", id_cpu);

    // CONEXION CLIENTE CON MEMORY STICK
    int conexion_stick = crear_conexion(logger, IP_MEMORY_STICK, PUERTO_MEMORY_STICK);
    log_info(logger, "> Modulo CPU Conectado a Memory Stick");



    // CONEXION CLIENTE CON KERNEL MEMORY
	int conexion_memory = crear_conexion(logger, IP_KERNEL_MEMORY, PUERTO_KERNEL_MEMORY);
    log_info(logger, "> Modulo CPU Conectado a Kernel Memory");


    // =========================================================
    // 🤝 HANDSHAKE CON EL MEMORY
    // =========================================================
    t_paquete* paquete_memoria = armar_paquete_scheduler_cpu(HANDSHAKE_CPU, &handshake_cpu);
    enviar_paquete(paquete_memoria, conexion_memory);
    eliminar_paquete(paquete_memoria);
    log_info(logger, "Handshake enviado a Kernel Memory (Soy CPU ID: %d)", id_cpu);


    //--------------------------------------------DESARROLLO CPU
    int pid_actual=1;
    bool procesando = true;
    t_registros registros = {0}; // Setea todos los campos en cero automáticamente


while (1) {
    log_info(logger, "CPU esperando proceso del Kernel...");
    
    // La CPU se bloquea acá hasta que el Kernel mande algo
    int cod_op = recibir_codigo_de_operacion(conexion_scheduler);
    
    switch (cod_op) {
        case NUEVO_PROCESO:
            //  nos mandaron un PID, lo leemos
            pid_actual = (int)recibir_pid_del_scheduler(conexion_scheduler);
            log_info(logger, "El Kernel me asignó el PID: %d", pid_actual);
            
            // el Contexto a la Memoria
            solicitar_contexto_a_memoria(conexion_memory, pid_actual);

            registros = recibir_contexto(conexion_memory);
            log_info(logger, "Contexto recibido. PC inicial: %d", registros.PC);
            
            procesando = true;
            while(procesando) {
                // 1. FETCH
                char* instruccion_proxima = fetch_instruccion(conexion_memory, pid_actual, registros.PC);
                log_info(logger, "## PID: %d - FETCH - Program Counter: %d", pid_actual, registros.PC);
                
                // 2. DECODE
                char** instruccion_separada = string_split(instruccion_proxima, " ");
                
                // 3. EXECUTE
                // La función devuelve true si debe seguir procesando, o false si hubo una Syscall (EXIT)
                procesando = ejecutar_instruccion(instruccion_separada, &registros, pid_actual, conexion_scheduler, conexion_memory, logger);

                // LIMPIEZA
                string_array_destroy(instruccion_separada);
                free(instruccion_proxima);

                // 4. CHECK INTERRUPT
                if (procesando) { 
                    int senial;
                    if (recv(conexion_scheduler, &senial, sizeof(int), MSG_DONTWAIT) > 0) {
                        log_info(logger, "## Interrupción recibida");
                        enviar_contexto_a_memoria(conexion_memory, pid_actual, &registros);
                        devolver_proceso_a_scheduler(conexion_scheduler, pid_actual, MOTIVO_INTERRUPCION);
                        procesando = false; 
                    }
                }
            }

            break;
            
        case -1:
            log_error(logger, "El Kernel se desconectó. Apagando CPU.");
            return EXIT_FAILURE;
            
        default:
            log_warning(logger, "Operacion desconocida del Kernel");
            break;
    }
}


    liberar_recursos(logger, config, conexion_scheduler, conexion_stick, conexion_memory);

    return 0;
}
