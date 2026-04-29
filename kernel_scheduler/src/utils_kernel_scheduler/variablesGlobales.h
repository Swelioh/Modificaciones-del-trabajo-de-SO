#ifndef SCHEDULER_GLOBALS_H_
#define SCHEDULER_GLOBALS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/queue.h>
#include <semaphore.h>
#include <unistd.h>
#include <serializacion/estructuras.h>
#include <serializacion/serializacion.h>


// **************************************************
//              Variables globales generales
// **************************************************
//Config
extern t_config* config;
extern char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
extern char* PLANIFICATION_ALGORITHM; //algoritmo de planificación a utilizar
extern char ** QUEUES_ALGORITHMS; //Lista de algoritmos a utilizar en CMN
extern int RR_QUANTUM; //Tiempo de quantum para RR en mS
extern char* QUEUE_PREEMPTION; //Desalojo entre colas habilitado (string con "TRUE" O "FALSE")
extern int SUSPENSION_TIMEOUT; //Tiempo para suspender un proceso que está en IO en mS
extern char* IP_KERNEL_MEMORY; //Ip para conectarse al modulo kernel memory
extern char* PUERTO_KERNEL_MEMORY; //Idem para puerto
extern char* PUERTO_KERNEL_SCHEDULER; //Puerto que utiliza el scheduler como servidor

//Logger
extern t_log* logger;


// **************************************************
//          Variables relacionadas a CPU
// **************************************************

//CPUs para usar
extern sem_t cpu_disponible;
//Cpus Tomadas    
extern sem_t cpu_tomadas;  

// Lista global de recursos de CPU
extern t_list* cpus_conectadas;

// Lista global de recursos de CPU
extern pthread_mutex_t mutex_lista_cpus;

// **************************************************
//          Variables relacionadas a IO
// **************************************************

typedef struct {
	uint32_t pid_proceso_relacionado; //Para saber que proceso fue el que "publico" esta tarea
	uint32_t tiempoSleepEnMs; // Tiempo en mS cuando la peticion fue tipo sleep
    // Info para cuando es tipo STDIN o STDOUT
	uint32_t registroDirecLogica; 
	uint32_t tamanioALeerEscribir;
}t_tarea_io;

extern t_queue* tareas_io_pendientes[3]; // Cada indice del vector de listas tiene las tareas pendientes a ser tomadas por la IO de ese tipo
                                        // tareas_io_pendientes[0] -> Tareas pendientes de modulos tipo SLEEP
                                        // tareas_io_pendientes[1] -> Tareas pendientes de modulos tipo STDIN
                                        // tareas_io_pendientes[2] -> Tareas pendientes de modulos tipo STDOUT

extern sem_t semaforo_tareas_io_pendientes[3]; // Semaforo tipo contador para asi bloquear a los hilos cuando no haya tareas de ese tipo para procesar
                                //
extern pthread_mutex_t mutex_lista_tareas_io[3]; // Semaforo tipo mutex para sincronizar la escritura y lectura dentro de cada instancia de la lista de tareas pendientes


#endif