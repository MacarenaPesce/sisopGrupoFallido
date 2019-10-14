#include <readline/readline.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>  //ver si es necesario
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



int pedir_oper_sacServer(char* ip, int puerto, char* pedido_oper, int codigo_oper) {

	char *tamaniomensajecasteado = malloc(1);
	char *caracter = malloc(1);
	char *mensaje = malloc(1);
	int tamaniomensaje = 0;

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY; //Cambiar por ip del servdor
	direccionservidor.sin_port = htons(1024); //cambiar por puerto

	int cliente = socket(AF_INET, SOCK_STREAM, 0);

	int servidor;

	if ((servidor = connect(cliente, (void*) &direccionservidor,
			sizeof(direccionservidor))) != 0) {
		perror("No me pude conectar.\n");

		return 1;
	}

	printf("Estoy aquí.\nMensaje para el servidor: ");

	char *mensaje = malloc(strlen(pedido_oper) + 2);
	sprintf(mensaje, "%c%s", codigo_oper, pedido_oper);
	free(pedido_oper);
	send(cliente, mensaje, sizeof(mensaje) + 1, 0);

	//Recibir la respuesta del servidor
	recv(cliente, mensaje, 1, 0);
	tamaniomensaje = *mensaje; //El tamaño está concatenado al mensaje que manda el servidor
	recv(cliente, mensaje, tamaniomensaje + 1, 0);
	mensaje[tamaniomensaje] = '\0';
	printf("%s\n", mensaje);

	free(mensaje);
	close(cliente);

	return 0;
}

