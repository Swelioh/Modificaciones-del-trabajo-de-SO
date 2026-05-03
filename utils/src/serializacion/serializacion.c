#include "estructuras.h"
#include "serializacion.h"

// ********************************************************************************
//                             MISC
// *********************************************************************************

// Crea el buffer que va a contener la informacion que se quiere enviar (El payload)
void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
	paquete->buffer->offset = 0;
}

void agregar_uint32_al_buffer(t_buffer *buffer, uint32_t variable)
{
	// Pido mas memoria dinamica para almacenar esta nueva variable
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint32_t)); // TODO: Habria que preguntar si se pudo realocar?
	// Copio en el stream (posicionandome al final) la nueva variable
	memcpy(buffer->stream + buffer->size, &variable, sizeof(uint32_t));
	// actualizo el tamanio total del buffer
	buffer->size += sizeof(uint32_t);
}
// Lee un uint32 del buffer y me adelanta el offset del buffer
uint32_t leer_uint32_del_buffer(t_buffer *buffer)
{
	uint32_t variable;
	// Utilizando el offset para posicionarme, me ubico
    memcpy(&variable, buffer->stream + buffer->offset, sizeof(uint32_t));
	// Actualizo el tamanio total del offset
    buffer->offset += sizeof(uint32_t);
	return variable;
}

void agregar_int32_al_buffer(t_buffer *buffer, int32_t variable)
{
	// Pido mas memoria dinamica para almacenar esta nueva variable
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(int32_t)); // TODO: Habria que preguntar si se pudo realocar?
	// Copio en el stream (posicionandome al final) la nueva variable
	memcpy(buffer->stream + buffer->size, &variable, sizeof(int32_t));
	// actualizo el tamanio total del buffer
	buffer->size += sizeof(int32_t);
}
// Lee un int32 del buffer y me adelanta el offset del buffer
int32_t leer_int32_del_buffer(t_buffer *buffer)
{
	int32_t variable;
	// Utilizando el offset para posicionarme, me ubico
    memcpy(&variable, buffer->stream + buffer->offset, sizeof(int32_t));
	// Actualizo el tamanio total del offset
    buffer->offset += sizeof(int32_t);
	return variable;
}

void agregar_uint8_al_buffer(t_buffer *buffer, uint8_t variable)
{
	// Pido mas memoria dinamica para almacenar esta nueva variable
	buffer->stream = realloc(buffer->stream, buffer->size + sizeof(uint8_t)); // TODO: Habria que preguntar si se pudo realocar?
	// Copio en el stream (posicionandome al final) la nueva variable
	memcpy(buffer->stream + buffer->size, &variable, sizeof(uint8_t));
	// actualizo el tamanio total del buffer
	buffer->size += sizeof(uint8_t);
}
// Lee un uint8 del buffer y me adelanta el offset del buffer
uint8_t leer_uint8_del_buffer(t_buffer *buffer)
{
	uint8_t variable;
	// Utilizando el offset para posicionarme, me ubico
    memcpy(&variable, buffer->stream + buffer->offset, sizeof(uint8_t));
	// Actualizo el tamanio total del offset
    buffer->offset += sizeof(uint8_t);
	return variable;
}

// Agrega un string al buffer. El largo_string ES SIN EL \0. 
void agregar_string_al_buffer(t_buffer *buffer, uint32_t largo_string, char *string)
{
	// Primero tengo que agregar al buffer el tamanio que va a ocupar el string (Sumo 1 para considerar el \0)
	agregar_int32_al_buffer(buffer, largo_string + 1);
	// Ahora pido mas memoria para almacenar el string
	buffer->stream = realloc(buffer->stream, buffer->size + largo_string + 1); // TODO: Habria que preguntar si se pudo realocar?
	// Copio en el stream (posicionandome al final) el string
	memcpy(buffer->stream + buffer->size, string, largo_string + 1);
	// actualizo el tamanio total del buffer
	buffer->size += (largo_string + 1);
}

// Lee el string del buffer. LUEGO HAY QUE LIBERAR LA MEMORIA DEL STRING
char *leer_string_del_buffer(t_buffer *buffer)
{
	int largo_string = leer_int32_del_buffer(buffer);
	char *string = malloc(largo_string);
	// Utilizando el offset para posicionarme, me ubico al comienzo del string y lo copio
    memcpy(string, buffer->stream + buffer->offset, largo_string);
	// Actualizo el tamanio total del offset
    buffer->offset += largo_string;
	return string;
}

// ********************************************************************************
//                           FUNCIONES PARA MANEJAR PAQUETES
// *********************************************************************************

// Crea la estructura del paquete que se va a enviar (Codigo de operacion + Buffer)
t_paquete* crear_paquete(codigo_operacion codigo)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo;
	crear_buffer(paquete);
	return paquete;
}

// Me libera toda la memoria que pedi para armar el paquete
void eliminar_paquete(t_paquete* paquete)
{
	if (paquete->buffer->stream != NULL) 
		free(paquete->buffer->stream);

	free(paquete->buffer);
	free(paquete);
}

t_paquete* recibir_paquete_completo(int socket) {

	//Reservamos memoria para el paquete a recibir
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));

    //Inicializamos offset
    paquete->buffer->offset = 0;

    //Recibimos el codigo de operacion, validamos que no devuelva <= 0 porque indicaria un error en recv
    if (recv(socket, &(paquete->codigo_operacion), sizeof(uint32_t), 0) <= 0) {
        free(paquete->buffer);
        free(paquete);
        return NULL;
    }

    //Recibimos tamaño del buffer
    if (recv(socket, &(paquete->buffer->size), sizeof(uint32_t), 0) <= 0) {
        free(paquete->buffer);
        free(paquete);
        return NULL;
    }

    if (paquete->buffer->size == 0) {
        return paquete;
    }

    //Reservamos memoria para el payload
    paquete->buffer->stream = malloc(paquete->buffer->size);

    //Recibimos el payload completo
    if (recv(socket, paquete->buffer->stream, paquete->buffer->size, 0) <= 0) {
        free(paquete->buffer->stream);
        free(paquete->buffer);
        free(paquete);
        return NULL;
    }

    return paquete;
}

// Esta funcion deja el paquete listo para enviar por el socket. Le pasas el operation code y luego la direccion de la estructura de datos que contiene la informacion que queres enviar.
t_paquete* armar_paquete(codigo_operacion codigo, void* struct_con_mensaje)
{
	t_paquete* paquete = crear_paquete(codigo);
	t_buffer* buffer = paquete -> buffer;
	buffer->offset = 0; //Lo inicializamos en 0 porque si no puede tener cualquier valor

	// Segun el codigo de operacion serializas el mensaje casteando tu estructura de datos a la correcta para ese mensaje que queres enviar
	switch(codigo)
	{
		case INGRESO_CPU:
			serializarIngresoCPU(buffer, *((t_ingreso_cpu*) struct_con_mensaje));
			break;
		case INGRESO_IO:
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

// Envia paquete al socket que se pasa por parametro, devuelve la cantidad de bytes enviados
int enviar_paquete(t_paquete* paquete, int socket_cliente)
{
	int bytes = paquete->buffer->size + 2*sizeof(int);
	void* a_enviar = serializar_paquete(paquete, bytes);

	int cant_bytes_enviados = send(socket_cliente, a_enviar, bytes, MSG_NOSIGNAL); // Ponemos el Flag de MSG no signal para que asi si se hace send a un socket que fue cerrado el SO no nos mate el proceso. Asi nosotros podemos trabajar ese error
	free(a_enviar);

	return cant_bytes_enviados;
}


// ********************************************************************************
//           FUNCIONES DE SERIALIACION Y DESERIALIZACION DE PAQUETES
// *********************************************************************************

void* serializar_paquete(t_paquete* paquete, int bytes)
{
    void * magic = malloc(bytes);
    int desplazamiento = 0;

    memcpy(magic + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
    desplazamiento+= sizeof(int);
    memcpy(magic + desplazamiento, &(paquete->buffer->size), sizeof(int));
    desplazamiento+= sizeof(int);

    //Solamente si tenemos un buffer que enviar lo agregamos en la serializacion
    if (paquete->buffer->size > 0) {
        memcpy(magic + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
        desplazamiento+= paquete->buffer->size;
    }

    return magic;
}