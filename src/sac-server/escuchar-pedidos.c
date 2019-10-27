#include "escuchar-pedidos.h"

int control_error_conexion(int bytes_leidos, struct sockaddr_in cliente,
		int fd_cliente) {

	if (bytes_leidos == 0) {
		printf("Se desconecto el cliente %s\n de %i\n",
				inet_ntoa(cliente.sin_addr), fd_cliente);
				return -1;
		//exit(1);
	}
	if (bytes_leidos < 0) {
		perror("recv");
		return -2;
		//exit(-1); CONSULTAR
	}
	return 0;
}

void* atender_pedidos(void* cliente_nuevo) {

	struct cliente_op* datos_cliente = cliente_nuevo;
	char* mensajeCliente;
	int tamanio;
	int nbytesTAM;
	int nbytesMSJ;
	char tamanioMSJ[1];

	while (1) {
		nbytesTAM = recv(datos_cliente->new_fd, tamanioMSJ, 1, 0);
		printf("bytes leidos al principio %i\n", nbytesTAM);
		if(control_error_conexion(nbytesTAM, datos_cliente->their_addr,
				datos_cliente->new_fd) == -1)
			break;

		int codigo_operacion = tamanioMSJ[0];
		printf("codigo de la operación: %i\n", codigo_operacion);

		nbytesTAM = recv(datos_cliente->new_fd, tamanioMSJ, 1, 0);
		if(control_error_conexion(nbytesTAM, datos_cliente->their_addr,
						datos_cliente->new_fd) == -1)
					break;
		tamanio = tamanioMSJ[0];
		printf("tamaño del mensaje %i bytes leidos %i\n", tamanio, nbytesTAM);

		mensajeCliente = malloc(tamanio + 1);
		printf("tamañoooo %i %i\n", tamanio, tamanio);
		nbytesMSJ = recv(datos_cliente->new_fd, mensajeCliente, tamanio, 0);
		printf("bytes del mensaje recibido %i\n", nbytesMSJ);
		if(control_error_conexion(nbytesMSJ, datos_cliente->their_addr,
						datos_cliente->new_fd) == -1)
					break;
		mensajeCliente[tamanio] = '\0';
		printf("%s dice >>> %s \n",
				inet_ntoa(datos_cliente->their_addr.sin_addr), mensajeCliente);

		//Le respondo al cliente
		char* lo_que_quiere = string_from_format(
				"Operación %i y su mensaje fue %s", codigo_operacion,
				mensajeCliente);
		char* respuesta = string_from_format("%c%s", strlen(lo_que_quiere),
				lo_que_quiere);
		printf("Al cliente le mando: %s\n", respuesta);
		if (send(datos_cliente->new_fd, respuesta, strlen(respuesta), 0) == -1) {
			perror("send\n");
			break;
		}

		printf("\n~~~~~~~~~~~~~~~~~~~~~~~\n");

	}
	close(datos_cliente->new_fd);
	//exit(0);
}

