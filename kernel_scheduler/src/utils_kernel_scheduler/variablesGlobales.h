#ifndef SCHEDULER_GLOBALS_H_
#define SCHEDULER_GLOBALS_H_

#include <commons/log.h>
#include <commons/config.h>
#include <semaphore.h>
#include <unistd.h>
#include <serializacion/estructuras.h>
#include <serializacion/serializacion.h>
#include <commons/collections/queue.h>

// **************************************************
//              Variables globales generales
// **************************************************
// Config
extern t_config *config;
extern char *LOG_LEVEL;               // Nivel de detalle máximo a mostrar.
extern char *PLANIFICATION_ALGORITHM; // algoritmo de planificación a utilizar
extern char **QUEUES_ALGORITHMS;      // Lista de algoritmos a utilizar en CMN
extern int RR_QUANTUM;                // Tiempo de quantum para RR en mS
extern char *QUEUE_PREEMPTION;        // Desalojo entre colas habilitado (string con "TRUE" O "FALSE")
extern int SUSPENSION_TIMEOUT;        // Tiempo para suspender un proceso que está en IO en mS
extern char *IP_KERNEL_MEMORY;        // Ip para conectarse al modulo kernel memory
extern char *PUERTO_KERNEL_MEMORY;    // Idem para puerto
extern char *PUERTO_KERNEL_SCHEDULER; // Puerto que utiliza el scheduler como servidor

// Logger
extern t_log *logger;

// Variable que almacena el FD que tiene el scheduler como servidor
extern int socket_scheduler;

extern volatile sig_atomic_t seguir_ejecutando; // TODO: Ver que onda esto, medio raro ese tipo

//
typedef enum
{
    NEW,
    READY,
    EXEC,
    BLOCK,
    SUSP_READY,
    SUSP_BLOCK,
    EXIT
} t_estado;

typedef struct
{
    uint32_t pid;
    t_estado estado;
    int prioridad;
    int prioridad_original;         // Esto para cuando quiera volver a la prioridad original
    struct timespec tiempo_llegada; // Tiempo de llegado al ultimo estado. Se usa para despachar pero capaz con una cola no hace falta
} t_pcb;

typedef struct {
    char path[256];
    int  prioridad;
    uint32_t pid;
} t_propuesta_PCB;

typedef struct
{
    t_queue *ProcesosDelEstado;
    pthread_mutex_t mutexLista;
} listaEstados;

extern listaEstados listaEstadosNew;
extern listaEstados listaEstadosReady;

extern sem_t semaforo_hay_propuestas_pcb;
extern sem_t semaforo_hay_pcb_ready;

// **************************************************
//          Variables relacionadas a CPU
// **************************************************

// CPUs para usar
extern sem_t cpu_disponible;
// Cpus Tomadas
extern sem_t cpu_tomadas;

// Lista global de recursos de CPU
extern t_list *cpus_conectadas;

// Lista global de recursos de CPU
extern pthread_mutex_t mutex_lista_cpus;

// **************************************************
//          Variables relacionadas a IO
// **************************************************

typedef struct
{
    uint32_t pid_proceso_relacionado; // Para saber que proceso fue el que "publico" esta tarea
    uint32_t tiempoSleepEnMs;         // Tiempo en mS cuando la peticion fue tipo sleep
    uint32_t registroDirecLogica;     // Info para cuando es tipo STDIN o STDOUT
    uint32_t tamanioALeerEscribir;
} t_tarea_io;

extern t_queue *tareas_io_pendientes[3]; // Cada indice del vector de listas tiene las tareas pendientes a ser tomadas por la IO de ese tipo
                                         // tareas_io_pendientes[0] -> Tareas pendientes de modulos tipo SLEEP
                                         // tareas_io_pendientes[1] -> Tareas pendientes de modulos tipo STDIN
                                         // tareas_io_pendientes[2] -> Tareas pendientes de modulos tipo STDOUT

extern sem_t semaforo_tareas_io_pendientes[3];   // Semaforo tipo contador para asi bloquear a los hilos cuando no haya tareas de ese tipo para procesar
                                                 //
extern pthread_mutex_t mutex_lista_tareas_io[3]; // Semaforo tipo mutex para sincronizar la escritura y lectura dentro de cada instancia de la lista de tareas pendientes

#endif