#include <utils/utils.h>

void saludar(char* quien) {
    printf("Hola desde %s!!\n", quien);
}

int crear_conexion(char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;
	int socket_cliente;
	int estado;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	estado = getaddrinfo(ip, puerto, &hints, &server_info);
	if (estado != 0) {
		fprintf(stderr, "Error en getaddrinfo: %s\n", gai_strerror(estado));
		return -1;
	}

	// Ahora vamos a crear el socket.
	socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (socket_cliente == -1) {
		perror("Error al crear el socket");
		freeaddrinfo(server_info);
		return -1;
	}

	// Ahora que tenemos el socket, vamos a conectarlo
	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
		perror("Error al conectar con el servidor");
		close(socket_cliente);
		freeaddrinfo(server_info);
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

int iniciar_servidor(t_log* logger,char* puerto)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, puerto, &hints, &servinfo);

	// Creamos el socket de escucha del servidor
    socket_servidor = socket(servinfo->ai_family,
                        servinfo->ai_socktype,
                        servinfo->ai_protocol);


    // //Esto creo que es para que podamos tener varios sockets al mismo tiempo...
    // setsockopt(fd_escucha, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));

    // Asociamos el socket a un puerto
    bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);

	// Asociamos el socket a un puerto
	listen(socket_servidor, SOMAXCONN);

	// Escuchamos las conexiones entrantes
    
	freeaddrinfo(servinfo);
	printf("Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log * logger)
{
	// Aceptamos un nuevo cliente
	int socket_cliente;
	socket_cliente = accept(socket_servidor, NULL, NULL);

	printf("Se conecto un cliente!");

	return socket_cliente;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");

	return nuevo_config;
}

void get_string_from_config(t_config* config, char* clave, char** valor)
{
    if(config_has_property(config,clave)){
        *valor = config_get_string_value(config, clave);
    }else{
        printf("FALTA CLAVE: %s\n", clave);
        exit(EXIT_FAILURE);
    }
}

void get_int_from_config(t_config* config, char* clave, int* valor)
{
    if(config_has_property(config,clave)){
        *valor = config_get_int_value(config, clave);
    }else{
        printf("FALTA CLAVE: %s\n", clave);
        exit(EXIT_FAILURE);
    }
}

void get_array_from_config(t_config* config, char* clave, char*** valor)
{
    if(config_has_property(config,clave)){
        *valor = config_get_array_value(config, clave);
    }else{
        printf("FALTA CLAVE: %s\n", clave);
        exit(EXIT_FAILURE);
    }
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

// FUNCIONES
// Crea la estructura del paquete que se va a enviar (Codigo de operacion + Buffer)
t_paquete* crear_paquete(op_code codigo)
{
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->codigo_operacion = codigo;
	crear_buffer(paquete);
	return paquete;
}
// Crea el buffer que va a contener la informacion que se quiere enviar (El payload)
void crear_buffer(t_paquete* paquete)
{
	paquete->buffer = malloc(sizeof(t_buffer));
	paquete->buffer->size = 0;
	paquete->buffer->stream = NULL;
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
    memcpy(string, buffer->stream + buffer->offset, sizeof(int32_t));
	// Actualizo el tamanio total del offset
    buffer->offset += largo_string;
	return string;
}
void eliminar_paquete(t_paquete* paquete)
{
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}

t_paquete* recibir_paquete_completo(int fd) {
    op_code codigo;
    if (recv(fd, &codigo, sizeof(op_code), MSG_WAITALL) <= 0)
        return NULL;

    uint32_t size;
    if (recv(fd, &size, sizeof(uint32_t), MSG_WAITALL) <= 0)
        return NULL;

    t_paquete* paquete = crear_paquete(codigo);
    paquete->buffer->size = size;

    if (size > 0) {
        paquete->buffer->stream = malloc(size);
        if (recv(fd, paquete->buffer->stream, size, MSG_WAITALL) <= 0) {
            eliminar_paquete(paquete);
            return NULL;
        }
    }

    paquete->buffer->offset = 0; 
    return paquete;
}