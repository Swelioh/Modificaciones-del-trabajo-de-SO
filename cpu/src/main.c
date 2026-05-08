#include <cpu_utils/cpu_utils.h>
#include <cpu_utils/inicializacion.h>

void liberar_recursos(t_log* logger, t_config* config, int conexion_scheduler, int conexion_stick, int conexion_memory)
{
    liberar_conexion(conexion_scheduler);
    liberar_conexion(conexion_stick);
    liberar_conexion(conexion_memory);
    log_destroy(logger);
    config_destroy(config);
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
    t_ingreso_cpu handshake_cpu;
    handshake_cpu.identificador_cpu = id_cpu;

    // Armamdo y enviado del paquete 
    t_paquete* paquete_ingreso = armar_paquete_scheduler_cpu(HANDSHAKE_CPU, &handshake_cpu);
    enviar_paquete(paquete_ingreso, conexion_scheduler);
    eliminar_paquete(paquete_ingreso);

    log_info(logger, "Handshake enviado a Kernel Scheduler (Soy CPU ID: %d)", id_cpu);

    // CONEXION CLIENTE CON MEMORY STICK
    int conexion_stick = crear_conexion(logger, ip, puerto_memory_stick);
    log_info(logger, "> Modulo CPU Conectado a Memory Stick");



    // CONEXION CLIENTE CON KERNEL MEMORY
	int conexion_memory = crear_conexion(logger, ip, puerto_kernel_memory);
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
                
                //FETCH
               char* instruccion_proxima = fetch_instruccion(conexion_memory, pid_actual, registros.PC);
                
                log_info(logger, "## PID: %d - FETCH - Program Counter: %d", pid_actual, registros.PC);
                
                //TODO: EXECUTE (HECHO, HAY QUE VER SI ESTA BIEN)
                // DECODE
               
                char operacion[20] = {0};
                char parametro1[20] = {0};
                char parametro2[20] = {0};
                
                //  palabras del string (ej: "SET" "AX" "10")
                // ¡Que pasa con EXIT? DUDA DE IMPLEMENTACION??
                sscanf(instruccion_proxima, "%s %s %s", operacion, parametro1, parametro2);
                // EXECUTE
                if (strcmp(operacion, "SET") == 0) {
                    // SET (Registro, Valor)
                    int valor = atoi(parametro2); // Convertimos el string del número a un int real
                    
                    
                    escribir_registro(&registros, parametro1, valor);
                   
                    log_info(logger, "## PID: %d - Ejecutando: %s - [%s, %s]", pid_actual, operacion, parametro1, parametro2);
                    
                    registros.PC++; 
                } 
               else if (strcmp(operacion, "SUM") == 0) {
                    // Lee el valor de ambos registros
                    uint32_t valor_destino = leer_registro(&registros, parametro1);
                    uint32_t valor_origen = leer_registro(&registros, parametro2);
                    
                    //  Escribe la suma en el primer registro
                    escribir_registro(&registros, parametro1, valor_destino + valor_origen);
                    
                    log_info(logger, "## PID: %d - Ejecutando: %s - [%s, %s]", pid_actual, operacion, parametro1, parametro2);
                    registros.PC++;
                }
                else if (strcmp(operacion, "JNZ") == 0) {
                    // JNZ evalúa el registro del parametro1, y salta a la instrucción del parametro2
                    uint32_t valor_registro = leer_registro(&registros, parametro1);
                    uint32_t salto = (uint32_t)atoi(parametro2);

                    log_info(logger, "## PID: %d - Ejecutando: %s - [%s, %s]", pid_actual, operacion, parametro1, parametro2);

                    if (valor_registro != 0) {
                        registros.PC = salto;
                    } else {
                        registros.PC++;       
                    }
                }
                else if (strcmp(operacion, "EXIT") == 0) {
                    log_info(logger, "## PID: %d - Ejecutando: %s", pid_actual, operacion);
                    procesando = false; // Rompe el ciclo
                }
                else {
                    log_error(logger, "PID: %d - Instruccion desconocida: %s", pid_actual, operacion);
                    procesando = false; // Rompe por seguridad
                }
                //LIBERO MEMORIA DE LA INSTRUCCION
                free(instruccion_proxima);

                
                // 4. CHECK INTERRUPT

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
