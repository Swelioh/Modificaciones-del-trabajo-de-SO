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
	// hints.ai_flags = AI_PASSIVE;

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
	log_trace(logger, "Listo para escuchar a mi cliente");

	return socket_servidor;
}

int esperar_cliente(int socket_servidor, t_log * logger)
{
	// Aceptamos un nuevo cliente
	int socket_cliente;
	socket_cliente = accept(socket_servidor, NULL, NULL);

	log_info(logger, "Se conecto un cliente!");

	return socket_cliente;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("../cliente.config");

	return nuevo_config;
}
