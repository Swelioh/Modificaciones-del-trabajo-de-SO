#ifndef INSTRUCCIONES_H_
#define INSTRUCCIONES_H_

#include <utils/utils.h>
#include <stdbool.h>
#include "cpu_utils.h"


// Función principal de Execute. 
// Devuelve 'true' si la CPU debe seguir ejecutando la próxima instrucción.
// Devuelve 'false' si hubo una Syscall (EXIT, SLEEP) o un error, y debe devolver el proceso.
bool ejecutar_instruccion(char** instruccion, t_registros* registros, int pid, int conexion_scheduler, int conexion_memory, t_log* logger);

// Funciones específicas
void instruccion_set(char** instruccion, t_registros* registros, int pid, t_log* logger);
void instruccion_sum(char** instruccion, t_registros* registros, int pid, t_log* logger);
void instruccion_jnz(char** instruccion, t_registros* registros, int pid, t_log* logger);

// Funciones que rompen el ciclo (Syscalls)
void instruccion_exit(int pid, t_registros* registros, int conexion_scheduler, int conexion_memory, t_log* logger);

#endif /* INSTRUCCIONES_H_ */