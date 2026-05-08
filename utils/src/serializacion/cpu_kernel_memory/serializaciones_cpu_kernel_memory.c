#include "serializaciones_cpu_kernel_memory.h"

// Esta funcion deja el paquete listo para enviar por el socket. Le pasas el operation code y luego la direccion de la estructura de datos que contiene la informacion que queres enviar.
t_paquete* armar_paquete_cpu_kernel_memory(codigo_operacion codigo, void* struct_con_mensaje)
{
	t_paquete* paquete = crear_paquete(codigo);
	t_buffer* buffer = paquete -> buffer;

	// Segun el codigo de operacion serializas el mensaje casteando tu estructura de datos a la correcta para ese mensaje que queres enviar
	switch(codigo)
	{
		case PEDIR_PROX_INSTRUCCION:
			serializar_prox_instruccion_cpu_kernel_memory(buffer, *((t_stream_prox_instruccion*) struct_con_mensaje));
			break;
		case RESPUESTA_PROX_INSTRUCCION:
			serializar_rta_prox_instruccion_cpu_kernel_memory(buffer, *((t_stream_rta_prox_instruccion*) struct_con_mensaje));
			break;
        case PEDIR_CONTEXTO: // <-- FALTABA 
            serializar_pedido_contexto_cpu_kernel_memory(buffer, *((t_stream_pedido_contexto*) struct_con_mensaje));
            break;
		default:
			printf("No se reconoce el codigo de operacion.");
			break;
	}
	return paquete;
}

// Esta funcion deja la estructura listo para usar dentro del handler. Le pasas el operation code y luego la direccion de la estructura de datos que contiene el mensaje a deserializar.
void desarmar_paquete_cpu_kernel_memory(codigo_operacion codigo, t_buffer* buffer, void* struct_con_mensaje)
{
	// Segun el codigo de operacion deserializas el mensaje casteando tu estructura de datos a la correcta para ese mensaje que recibiste
	switch(codigo)
	{
		case PEDIR_PROX_INSTRUCCION:
			deserializar_prox_instruccion_cpu_kernel_memory(buffer, (t_stream_prox_instruccion*) struct_con_mensaje);			
			break;
        case RESPUESTA_PROX_INSTRUCCION: // <-- FALTABA 
            deserializar_rta_prox_instruccion_cpu_kernel_memory(buffer, (t_stream_rta_prox_instruccion*) struct_con_mensaje);           
            break;
        case PEDIR_CONTEXTO: // <-- FALTABA 
            deserializar_pedido_contexto_cpu_kernel_memory(buffer, (t_stream_pedido_contexto*) struct_con_mensaje);
            break;    
		default:
			printf("No se reconoce el codigo de operacion.");
			break;
	}
}

// Mensaje que envia CPU a MEMORY para pedir prox instruccion
void serializar_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_prox_instruccion struct_a_serializar)
{
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pid);
	agregar_uint32_al_buffer(buffer, struct_a_serializar.pc);
}

void deserializar_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_prox_instruccion* struct_donde_deserializo)
{
	struct_donde_deserializo->pid = leer_uint32_del_buffer(buffer);
	struct_donde_deserializo->pc = leer_uint32_del_buffer(buffer);
}

// Mensaje que envia MEMORY a CPU para responder la prox instruccion
void serializar_rta_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_rta_prox_instruccion struct_a_serializar)
{
	agregar_string_al_buffer(buffer, struct_a_serializar.largo_instruccion, struct_a_serializar.instruccion);
}

void deserializar_rta_prox_instruccion_cpu_kernel_memory(t_buffer *buffer, t_stream_rta_prox_instruccion* struct_donde_deserializo)
{
    // Leemos el tamaño y luego el string dinámico
    struct_donde_deserializo->largo_instruccion = leer_uint32_del_buffer(buffer);
    struct_donde_deserializo->instruccion = leer_string_del_buffer(buffer);
}

// ======================================================================
// PEDIDO DE CONTEXTO (CPU -> Memoria)
// ======================================================================
void serializar_pedido_contexto_cpu_kernel_memory(t_buffer *buffer, t_stream_pedido_contexto struct_a_serializar)
{
    agregar_uint32_al_buffer(buffer, struct_a_serializar.pid);
}

void deserializar_pedido_contexto_cpu_kernel_memory(t_buffer *buffer, t_stream_pedido_contexto* struct_donde_deserializo)
{
    struct_donde_deserializo->pid = leer_uint32_del_buffer(buffer);
}