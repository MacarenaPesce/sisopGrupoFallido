
#include <escuchar-pedidos.h>



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

	int cont = 0;
	while (1) {  // main accept() loop
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size))
				== -1) {
			perror("accept");
			continue;
		}
		printf("\nserver: got connection from %s in %i\n",
				inet_ntoa(their_addr.sin_addr), new_fd); //PREGUNTA: ¿ESTA ESTRUCTURA ES COPARTIDA?

		//A p_thread pasarle new_fd, their_addr
		struct cliente_op nuevo_cliente;
		nuevo_cliente.new_fd = new_fd;
		nuevo_cliente.their_addr = their_addr; //PUEDE QUE SEA LOCAL...

		pthread_t thread_escuchar;
		pthread_attr_t atributos;
		pthread_attr_init(&atributos);
		pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_DETACHED); //PTHREAD_CREATE_DETACHED
		// Acá creo el hilo
		int estado = pthread_create(&thread_escuchar, &atributos,
				atender_pedidos, &nuevo_cliente);
		printf("estado %i\n", estado);

		printf("\nwhile número %i\n", cont);
		cont++;
	}

	printf("me salí del while\n");
	return 0;

}
