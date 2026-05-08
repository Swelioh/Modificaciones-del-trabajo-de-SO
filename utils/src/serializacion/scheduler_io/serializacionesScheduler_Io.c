#include "serializacionesScheduler_Io.h"

// Esta funcion deja el paquete listo para enviar por el socket. Le pasas el operation code y luego la direccion de la estructura de datos que contiene la informacion que queres enviar.
t_paquete* armar_paquete_scheduler_io(codigo_operacion codigo, void* struct_con_mensaje)
{
	t_paquete* paquete = crear_paquete(codigo);
	t_buffer* buffer = paquete -> buffer;

	// Segun el codigo de operacion serializas el mensaje casteando tu estructura de datos a la correcta para ese mensaje que queres enviar
	switch(codigo)
	{
		case HANDSHAKE_IO:
			serializarIngresoIO(buffer, *((t_ingreso_io*) struct_con_mensaje));
			break;
		case A_LA_ESPERA_IO:
			// Solo mandamos el codigo, por lo que no hace falta serializar ningun buffer
			break;
		case SOLICITUD_IO_SLEEP:
			serializarSolicitudIoSleep(buffer, *((t_solicitud_io_sleep*) struct_con_mensaje));
			break;
		case REGRESO_IO_SLEEP:
			serializarRegresoIOSleep(buffer, *((t_regreso_io_sleep*) struct_con_mensaje));
			break;
		case SOLICITUD_IO_STDIN:
			serializarSolicitudIoStdin(buffer, *((t_solicitud_io_stdin*) struct_con_mensaje));
			break;
		case REGRESO_IO_STDIN:
			serializarRegresoIOStdin(buffer, *((t_regreso_io_stdin*) struct_con_mensaje));
			break;
		case REGRESO_IO_STDOUT:
			serializarRegresoIOStdout(buffer, *((t_regreso_io_stdout*) struct_con_mensaje));
			break;
		default:
			printf("No se reconoce el codigo de operacion.");
			break;
	}
	return paquete;
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

// Mensaje que envia SCHEDULER a IO indicando la cadena a imprimir
void serializarSolicitudIoStdout(t_buffer *buffer, t_solicitud_io_stdout struct_a_serializar)
{
    agregar_uint32_al_buffer(buffer, struct_a_serializar.pid_proceso);
    agregar_string_al_buffer(buffer, struct_a_serializar.largo_cadena, struct_a_serializar.cadena);
}
void deserializarSolicitudIoStdout(t_buffer *buffer, t_solicitud_io_stdout* struct_donde_deserializo)
{
    struct_donde_deserializo->pid_proceso = leer_uint32_del_buffer(buffer);
    struct_donde_deserializo->cadena = leer_string_del_buffer(buffer);
}