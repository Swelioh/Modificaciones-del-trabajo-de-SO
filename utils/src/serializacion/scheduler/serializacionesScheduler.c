#include "serializacionesScheduler.h"

// Mensaje que envia CPU al SCHEDULER avisando que se dispone de una CPU nueva para el procesamiento de datos
void serializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.identificador_cpu);
}
void deserializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu* struct_donde_deserializo)
{
	struct_donde_deserializo->identificador_cpu = leer_uint32_del_buffer(buffer);
}

// Mensaje que envia IO al SCHEDULER avisando que tipo de modulo fue el que se conecto
void serializarIngresoIO(t_buffer *buffer, t_ingreso_io struct_a_serializar)
{
	agregar_uint8_al_buffer(buffer, struct_a_serializar.tipo_io);
}
void deserializarIngresoIO(t_buffer *buffer, t_ingreso_io* struct_donde_deserializo)
{
	struct_donde_deserializo->tipo_io = leer_uint8_del_buffer(buffer);
}

// Mensaje que envia SCHEDULER a IO indicando el tiempo de sleep en mS
void serializarSolicitudIoSleep(t_buffer *buffer, t_solicitud_io_sleep struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pid_proceso);
	agregar_uint32_al_buffer(buffer, struct_a_serializar.tiempo_sleep_ms);
}
void deserializarSolicitudIoSleep(t_buffer *buffer, t_solicitud_io_sleep* struct_donde_deserializo)
{
	struct_donde_deserializo->pid_proceso = leer_uint32_del_buffer(buffer);
	struct_donde_deserializo->tiempo_sleep_ms = leer_uint32_del_buffer(buffer);
}

// Mensaje que envia IO a SCHEDULER indicando que se termino la operacion de SLEEP
void serializarRegresoIOSleep(t_buffer *buffer, t_regreso_io_sleep struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pid_proceso);
}
void deserializarRegresoIOSleep(t_buffer *buffer, t_regreso_io_sleep* struct_donde_deserializo)
{
	struct_donde_deserializo->pid_proceso = leer_uint32_del_buffer(buffer);
}

// Mensaje que envia SCHEDULER a IO indicando el tiempo de sleep en mS
void serializarSolicitudIoStdin(t_buffer *buffer, t_solicitud_io_stdin struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pid_proceso);
	agregar_uint32_al_buffer(buffer, struct_a_serializar.tamanioALeer);
}
void deserializarSolicitudIoStdin(t_buffer *buffer, t_solicitud_io_stdin* struct_donde_deserializo)
{
	struct_donde_deserializo->pid_proceso = leer_uint32_del_buffer(buffer);
	struct_donde_deserializo->tamanioALeer = leer_uint32_del_buffer(buffer);
}
//Mensaje que envia IO a SCHEDULER indicando que se termino la operacion de STDIN
void serializarRegresoIOStdin(t_buffer *buffer, t_regreso_io_stdin struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pid_proceso);
	agregar_string_al_buffer(buffer, struct_a_serializar.largo_cadena, struct_a_serializar.cadena);
}
void deserializarRegresoIOStdin(t_buffer *buffer, t_regreso_io_stdin* struct_donde_deserializo)
{
	struct_donde_deserializo->pid_proceso = leer_uint32_del_buffer(buffer);
	struct_donde_deserializo->cadena = leer_string_del_buffer(buffer);
}

// Mensaje que envia IO a SCHEDULER indicando que se termino la operacion de STDOUT
void serializarRegresoIOStdout(t_buffer *buffer, t_regreso_io_stdout struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pid_proceso);
}
void deserializarRegresoIOStdout(t_buffer *buffer, t_regreso_io_stdout* struct_donde_deserializo)
{
	struct_donde_deserializo->pid_proceso = leer_uint32_del_buffer(buffer);
}