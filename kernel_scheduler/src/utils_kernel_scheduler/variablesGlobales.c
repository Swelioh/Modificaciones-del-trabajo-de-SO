#include "variablesGlobales.h"

// *********************************************
//              Variables globales
// *********************************************
//Config
t_config* config;
char* LOG_LEVEL; //Nivel de detalle máximo a mostrar.
char* PLANIFICATION_ALGORITHM; //algoritmo de planificación a utilizar
char ** QUEUES_ALGORITHMS; //Lista de algoritmos a utilizar en CMN
int RR_QUANTUM; //Tiempo de quantum para RR en mS
char* QUEUE_PREEMPTION; //Desalojo entre colas habilitado (string con "TRUE" O "FALSE")
int SUSPENSION_TIMEOUT; //Tiempo para suspender un proceso que está en IO en mS
char* IP_KERNEL_MEMORY; //Ip para conectarse al modulo kernel memory
char* PUERTO_KERNEL_MEMORY; //Idem para puerto
char* PUERTO_KERNEL_SCHEDULER; //Puerto que utiliza el scheduler como servidor

//Logger
t_log* logger;

// Variable que almacena el FD que tiene el scheduler como servidor
int socket_scheduler;

volatile sig_atomic_t seguir_ejecutando = 1;

listaEstados listaEstadosNew;
listaEstados listaEstadosReady;

sem_t semaforo_hay_propuestas_pcb;
sem_t semaforo_hay_pcb_ready;

// **************************************************
//          Variables relacionadas a CPU
// **************************************************

// CPUs para usar
sem_t cpu_disponible;
// Cpus Tomadas
sem_t cpu_tomadas;

// Lista global de recursos de CPU
t_list *cpus_conectadas;

// Lista global de recursos de CPU
pthread_mutex_t mutex_lista_cpus;

// **************************************************
//          Variables relacionadas a IO
// **************************************************
t_queue* tareas_io_pendientes[3];
sem_t semaforo_tareas_io_pendientes[3];
pthread_mutex_t mutex_lista_tareas_io[3]; 

