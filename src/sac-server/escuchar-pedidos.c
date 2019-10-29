#include "escuchar-pedidos.h"

void control_error_conexion(int bytes_leidos, struct sockaddr_in cliente,
		int fd_cliente) {

	if (bytes_leidos == 0) {
		printf("Se desconecto el cliente %s\n de %i\n",
				inet_ntoa(cliente.sin_addr), fd_cliente);
				pthread_exit (NULL);
				//exit(1);
	}
	if (bytes_leidos < 0) {
		perror("recv");
		pthread_exit (NULL);
		//exit(-1); CONSULTAR
	}
}

int largo_archivo(FILE* archivo) {
	int largo;
	fseek(archivo, 0, SEEK_END);
	largo = ftell(archivo);
	fseek(archivo, 0, SEEK_SET);
	printf("\nlargo_archivo: %i\n", largo);

	return largo;
}

char* sac_leer(char* path) {

	int largo;
	char* buffer;

	FILE* archivo = fopen(path, "r");
	largo = largo_archivo(archivo);
	buffer = malloc(largo);
	fread(buffer, 1, largo, archivo);

	return buffer;
}

void* atender_pedidos(void* cliente_nuevo) {

	printf("\n ~~~~ Hola desde el hilo %ld ~~~~ \n", pthread_self());

	struct cliente_op* datos_cliente = cliente_nuevo;
	char* mensajeCliente;
	int tamanio;
	int nbytesTAM;
	int nbytesMSJ;
	char tamanioMSJ[1];

	while (1) {
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

		char* lo_que_quiere;

		switch(codigo_operacion) {
		case LEER_ARCHIVO:
			strcpy(lo_que_quiere, sac_leer("archivo.txt"));
			break;
		}
		//Le respondo al cliente

		char* respuesta = string_from_format("%c%s", strlen(lo_que_quiere),
				lo_que_quiere);

		printf("Al cliente le mando: %s\n", respuesta);
		if (send(datos_cliente->new_fd, respuesta, strlen(respuesta), 0) == -1) {
			perror("send\n");
			pthread_exit (NULL);
		}

		printf("\n~~~~~~~~~~~~~~~~~~~~~~~\n");
	}
	close(datos_cliente->new_fd);
	pthread_exit (NULL);
	//exit(0);
}

