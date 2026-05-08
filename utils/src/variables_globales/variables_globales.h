#ifndef VARIABLES_GLOBALES_H_
#define VARIABLES_GLOBALES_H_

typedef enum {
    HANDSHAKE_CPU = 1, //es mio TIAGO
    HANDSHAKE_IO  = 2,
    HANDSHAKE_KERNEL_SCHEDULER = 3,
    HANDSHAKE_MEMORY_STICK = 4,
    HANDSHAKE_SWAP = 5,

    OP_CREAR_PROCESO = 10,
    OP_FIN_PROCESO = 11,
    OP_SYSCALL     = 12,
    OP_INTERRUPCION= 13,

    OP_IO_FIN      = 20,

    OP_EJECUTAR_PROCESO = 30,
    // CODIGOS DEL SCHEDULER - IO

     
    // CODIGOS DEL SCHEDULER - MEMORY
    CREAR_PROCESO = 31,
    WRITE_MEMORY = 32,
    READ_MEMORY = 33,
    ESPACIO_LIBRE_EN_MEMORIA = 34,

    // CODIGOS DEL SCHEDULER - CPU
    PEDIR_PROX_INSTRUCCION = 35,
    RESPUESTA_PROX_INSTRUCCION = 36,
    GUARDAR_CONTEXTO = 37,
    OBTENER_CONTEXTO = 38,

	  // CODIGOS DEL SCHEDULER - IO
	A_LA_ESPERA_IO = 100,
	SOLICITUD_IO_SLEEP,
	REGRESO_IO_SLEEP,
	SOLICITUD_IO_STDIN,
	REGRESO_IO_STDIN,
	SOLICITUD_IO_STDOUT,
	REGRESO_IO_STDOUT,

    // STATUS REPONSE
    STATUS_OK = 200,
    STATUS_FAILED = 400,


    //CODIGOS DEL CPU-SCHEDULER
    NUEVO_PROCESO,       // El Kernel te manda un PID para ejecutar
    DESALOJO_PROCESO,    // La CPU le devuelve el PID al Kernel
    INTERRUPCION,        // El Kernel le dice a la CPU que frene
    MENSAJE,              // Para mandar strings simples (como el Handshake) CAMBIAR
    PEDIR_CONTEXTO,      // CPU le dice a Memoria: "Dame los registros del PID X"
    CONTEXTO_ACTUALIZADO, // Memoria le responde a CPU con los registros
     
    //CODIGOS DEL CPU-MEMORY
    
    PEDIR_INSTRUCCION  //Para el Fetch
} codigo_operacion;



#endif