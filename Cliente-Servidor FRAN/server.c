#include"server.h"

int createServer(char* IP, char *PORT) {

	int server_socket;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PORT, &hints, &servinfo);

	for (p=servinfo; p != NULL; p = p->ai_next)
	{
		if ((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if (bind(server_socket, p->ai_addr, p->ai_addrlen) == -1) {
			close(server_socket);
			continue;
		}
		break;
	}

	listen(server_socket, SOMAXCONN);

	freeaddrinfo(servinfo);

	return server_socket;
}

int connectToClient(int listeningSocket) {
	struct sockaddr_in client_addr;
	socklen_t addr_size = sizeof(struct sockaddr_in);

	int client_socket = accept(listeningSocket, (void*) &client_addr, &addr_size);
	if(client_socket < 0) {
		perror("accept error : ");
	}

	return client_socket;
}

int readQueryFromClient(int client, char *package) {

	int status = recv(client, (void*) package, PACKAGESIZE, 0);
	if(strcasecmp("exit\n", package) == 0) return -1;

	return status;

}

void closeServer(int server) {
	close(server);
}


int main(){

	 //Estas y otras preguntas existenciales son resueltas getaddrinfo();
	 //Obtiene los datos de la direccion de red y lo guarda en serverInfo.

	struct addrinfo hints;
	struct addrinfo *serverInfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;		// No importa si uso IPv4 o IPv6
	hints.ai_flags = AI_PASSIVE;		// Asigna el address del localhost: 127.0.0.1
	hints.ai_socktype = SOCK_STREAM;	// Indica que usaremos el protocolo TCP

	getaddrinfo(NULL, PUERTO, &hints, &serverInfo); // Notar que le pasamos NULL como IP, ya que le indicamos que use localhost en AI_PASSIVE


	//Mediante socket(), obtengo el File Descriptor que me proporciona el sistema (un integer identificador).

	// Necesitamos un socket que escuche las conecciones entrantes

	int listenningSocket;
	listenningSocket = socket(serverInfo->ai_family, serverInfo->ai_socktype, serverInfo->ai_protocol);

	//Le digo al sistema que voy a utilizar el archivo que me proporciono para escuchar las conexiones por un puerto especifico.
	 
	bind(listenningSocket,serverInfo->ai_addr, serverInfo->ai_addrlen);
	freeaddrinfo(serverInfo); // Ya no lo vamos a necesitar


	listen(listenningSocket, BACKLOG);		// IMPORTANTE: listen() es una syscall BLOQUEANTE.

	//Aceptamos la conexion entrante, y creamos un nuevo socket mediante el cual nos podamos comunicar (que no es mas que un archivo).

	/*	¿Por que crear un nuevo socket? Porque el anterior lo necesitamos para escuchar las conexiones entrantes. De la misma forma que
	 *	uno no puede estar hablando por telefono a la vez que esta esperando que lo llamen, un socket no se puede encargar de escuchar
	 *	las conexiones entrantes y ademas comunicarse con un cliente.
	 *
	 */

	struct sockaddr_in addr;		// Esta estructura contendra los datos de la conexion del cliente. IP, puerto, etc.
	socklen_t addrlen = sizeof(addr);

	int socketCliente = accept(listenningSocket, (struct sockaddr *) &addr, &addrlen);

	/*
	 * 	Recibir paquetes de nuestro cliente...
	 * 	Vamos a ESPERAR (ergo, funcion bloqueante) que nos manden los paquetes, y los imprimieremos por pantalla.
	 *	Cuando el cliente cierra la conexion, recv() devolvera 0.
	 */

	char package[PACKAGESIZE];
	int status = 1;		// Estructura que manjea el status de los recieve.

	printf("Cliente conectado. Esperando mensajes:\n");

	while (status != 0){
		status = recv(socketCliente, (void*) package, PACKAGESIZE, 0);
		if (status != 0) printf("%s", package);

	}
	
	//Terminado el intercambio de paquetes, cerramos todas las conexiones.
	close(socketCliente);
	close(listenningSocket);


	return 0;
}





