#include "escuchar-pedidos.h"

void control_error_conexion(int bytes_leidos, struct sockaddr_in cliente,
		int fd_cliente) {

	if (bytes_leidos == 0) {
		printf("Se desconecto el cliente %s\n de %i\n",
				inet_ntoa(cliente.sin_addr), fd_cliente);
		exit(1);
	}
	if (bytes_leidos < 0) {
		perror("recv");
		exit(-1);
	}
}

void* atender_pedidos(void* cliente_nuevo) {

	struct cliente_op* datos_cliente = cliente_nuevo;
	char* mensajeCliente;
	int tamanio;
	int nbytesTAM;
	int nbytesMSJ;
	char tamanioMSJ[1];



		nbytesTAM = recv(datos_cliente->new_fd, tamanioMSJ, 1, 0);
		printf("bytes leidos al principio %i\n", nbytesTAM);
		control_error_conexion(nbytesTAM, datos_cliente->their_addr,
				datos_cliente->new_fd);
		int codigo_operacion = tamanioMSJ[0];
		printf("codigo de la operación: %i\n", codigo_operacion);

		nbytesTAM = recv(datos_cliente->new_fd, tamanioMSJ, 1, 0);
		control_error_conexion(nbytesTAM, datos_cliente->their_addr,
				datos_cliente->new_fd);
		tamanio = tamanioMSJ[0];
		printf("tamaño del mensaje %i bytes leidos %i\n", tamanio, nbytesTAM);



		mensajeCliente = malloc(tamanio + 1);
		printf("tamañoooo %i %i\n", tamanio, tamanio);
		nbytesMSJ = recv(datos_cliente->new_fd, mensajeCliente, tamanio, 0);
		printf("bytes del mensaje recibido %i\n", nbytesMSJ);
		control_error_conexion(nbytesMSJ, datos_cliente->their_addr,
				datos_cliente->new_fd);
		mensajeCliente[tamanio] = '\0';
		printf("%s dice >>> %s \n",
				inet_ntoa(datos_cliente->their_addr.sin_addr), mensajeCliente);

		//Le respondo al cliente
		char* lo_que_quiere = string_from_format("Operación %i y su mensaje fue %s",
				codigo_operacion, mensajeCliente);
		char* respuesta = string_from_format("%c%s", strlen(lo_que_quiere), lo_que_quiere);
		printf("Al cliente le mando: %s\n", respuesta);
		if (send(datos_cliente->new_fd, respuesta, strlen(respuesta), 0)
				== -1)
			perror("send\n");


	close(datos_cliente->new_fd);
	exit(0);
}

int main(void) {
	int sockfd, new_fd; // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
	struct sockaddr_in my_addr;    // información sobre mi dirección
	struct sockaddr_in their_addr; // información sobre la dirección del cliente
	int sin_size;
	int yes = 1;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}

	my_addr.sin_family = AF_INET;         // Ordenación de bytes de la máquina
	my_addr.sin_port = htons(MYPORT);    // short, Ordenación de bytes de la red
	my_addr.sin_addr.s_addr = INADDR_ANY; //inet_addr("192.168.43.135"); // Rellenar con mi dirección IP
	memset(&(my_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura

	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
			== -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	printf("Estoy escuchando, invita a tus amigos al server %s en el "
			"puerto %d\n", inet_ntoa(my_addr.sin_addr), MYPORT);

	while (1) {  // main accept() loop
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size))
				== -1) {
			perror("accept");
			continue;
		}
		printf("\nserver: got connection from %s in %i\n",
				inet_ntoa(their_addr.sin_addr), new_fd);

		//A p_thread pasarle new_fd, their_addr
		struct cliente_op nuevo_cliente;
		nuevo_cliente.new_fd = new_fd;
		nuevo_cliente.their_addr = their_addr;

		pthread_t thread_escuchar;
		pthread_attr_t atributos;
		pthread_attr_init(&atributos);
		//pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE); //PTHREAD_CREATE_DETACHED
		// Acá creo el hilo
		int estado = pthread_create(&thread_escuchar, NULL, atender_pedidos,
				&nuevo_cliente);
		printf("estado %i\n", estado);
	}

	return 0;

}
