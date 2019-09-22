/*
 ** client.c -- Ejemplo de cliente de sockets de flujo
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <readline/readline.h>

#define PORT 3491 // puerto al que vamos a conectar

#define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez

int main() {
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	//struct hostent *he;
	struct sockaddr_in their_addr; // información de la dirección de destino

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
	their_addr.sin_port = htons(PORT);  // short, Ordenación de bytes de la red
	their_addr.sin_addr.s_addr = INADDR_ANY;  //*((struct in_addr *)he->h_addr);
	memset(&(their_addr.sin_zero), '\0', 8); // poner a cero el resto de la estructura

	if (connect(sockfd, (struct sockaddr *) &their_addr,
			sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	//Escuchar el mensaje de bienvenida
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE - 1, 0)) == -1) {
		perror("recv");
		exit(1);
	}
	buf[numbytes] = '\0';
	printf("Received: %s", buf);

	//Enviar mensajes al servidor
	int max = 255;
	char mensajeServer[max + 1];
	char tamanio[2]; //max 255
	char* linea = malloc(max);

	do {
		linea = readline("<<< ");
		tamanio[0] = strlen(linea);
		tamanio[1] = '\0';
		sprintf(mensajeServer, "%s%s", tamanio, linea);
		send(sockfd, mensajeServer, strlen(mensajeServer), 0);
		free(linea);
	} while (linea != NULL);

	close(sockfd);

	return 0;
}
