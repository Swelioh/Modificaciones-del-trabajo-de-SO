#include "serializacionesScheduler_Cpu.h"


// Esta funcion deja el paquete listo para enviar por el socket. Le pasas el operation code y luego la direccion de la estructura de datos que contiene la informacion que queres enviar.
t_paquete* armar_paquete_scheduler_cpu(codigo_operacion codigo, void* struct_con_mensaje)
{
	t_paquete* paquete = crear_paquete(codigo);
	t_buffer* buffer = paquete -> buffer;

	// Segun el codigo de operacion serializas el mensaje casteando tu estructura de datos a la correcta para ese mensaje que queres enviar
	switch(codigo)
	{
		case HANDSHAKE_CPU:
			serializarIngresoCPU(buffer, *((t_ingreso_cpu*) struct_con_mensaje));
			break;
		default:
			printf("No se reconoce el codigo de operacion.");
			break;
	}
	return paquete;
}

// Mensaje que envia CPU al SCHEDULER avisando que se dispone de una CPU nueva para el procesamiento de datos
void serializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.identificador_cpu);
}
void deserializarIngresoCPU(t_buffer *buffer, t_ingreso_cpu* struct_donde_deserializo)
{
	struct_donde_deserializo->identificador_cpu = leer_uint32_del_buffer(buffer);
}