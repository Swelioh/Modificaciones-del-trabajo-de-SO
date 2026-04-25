#include <utils/utils.h>

// ********************************************************************************
//                   FUNCIONES PARA GESTIONAR CONEXIONES
// ********************************************************************************

//Inicia la conexion con el servidor que esta escuchando en esa IP y PUERTO. Nos devuelve un fd del socket del cliente
int crear_conexion(t_log* logger, char* ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;
	int socket_cliente;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if (getaddrinfo(ip, puerto, &hints, &server_info) != 0) {
		log_error(logger, "No se pudo hacer el getaddrinfo al momento de conectarse el servidor.");
		return -1;
	}

	// Le pedimos al SO que nos de un socket para asi poder comunicarnos con otro proceso
	socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
	if (socket_cliente == -1) {
        log_error(logger, "Hubo un error al crear el socket.");
		freeaddrinfo(server_info);
		return -1;
	}

	// Iniciamos la conexion con el servidor
	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
        log_error(logger, "Hubo un error al conectarse con el servidor.");
		close(socket_cliente);
		freeaddrinfo(server_info);
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}

//Cierra el fd del socket utilizado para la conexion
void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}

// Inicia el servidor escuchando al puerto que le pasas como parametro y devuelve el fd del socket del servidor
int iniciar_servidor(t_log* logger,char* puerto)
{
	int socket_servidor;

	struct addrinfo hints, *servinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, puerto, &hints, &servinfo) != 0) {
        log_error(logger, "No se pudo hacer el getaddrinfo al momento de iniciar el servidor.");
        return -1;
    }

	// Le pedimos al SO que nos de un socket para asi poder comunicarnos con otro proceso
    socket_servidor = socket(servinfo->ai_family,
                        servinfo->ai_socktype,
                        servinfo->ai_protocol);

	if (socket_servidor == -1) {
        log_error(logger, "Hubo un error al crear el socket.");
        freeaddrinfo(servinfo);
        return -1;
    }

	// Configuramos el comportamiento del socket para que asi varios sockets puedan usar el mismo puerto (Para que no nos diga AddressAlreadyInUse cuando cerramos el server y lo abrimos al toque)
	if (setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int)) == -1) {
        log_error(logger, "Error al configurar el socket.");
        close(socket_servidor);
        freeaddrinfo(servinfo);
        return -1;
    }

    // Asociamos el socket a un puerto en especifico donde vamos a escuchar
	if (bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        log_error(logger, "Hubo un error al asociar el socket al puerto.");
        close(socket_servidor);
        freeaddrinfo(servinfo);
        return -1;
    }

	// Ahora el socket queda esperando nuevas conexiones
	if (listen(socket_servidor, SOMAXCONN) == -1) {
        log_error(logger, "Hubo al escuchar en el puerto.");
        close(socket_servidor);
        freeaddrinfo(servinfo);
        return -1;
    }

	// Liberamos la memoria que pedimos con getaddrinfo
	freeaddrinfo(servinfo);
	printf("Escuchando por nuevos clientes en el puerto: %s\n", puerto);

	return socket_servidor;
}

// (BLOQUEANTE) - Espera a la conexion de un cliente
int esperar_cliente(int socket_servidor, t_log * logger)
{
	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);

	// printf("Se conecto un cliente!"); // ESTO DEBERIA IR EN EL CODIGO DEL SERVIDOR (Porque son de los logs obligatorios decir QUIEN se conecto)

	return socket_cliente;
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

// ********************************************************************************
//                   FUNCIONES PARA ARCHIVOS DE CONFIGURACION
// ********************************************************************************
t_config* abrirConfig(char* pathAlConfig)
{
	t_config* config = config_create(pathAlConfig);
	if(config == NULL){
        printf("No se pudo inicializar el archivo de config, por favor revise el path. Actual: '%s'\n", pathAlConfig);
        exit(EXIT_FAILURE) ;
    }
}

// Me obtiene el string del config. NO HACE FALTA LIBERAR LA MEMORIA
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
