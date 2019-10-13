#include <readline/readline.h>
#include <commons/string.h>
#include <commons/config.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void castear (int numero, char *digitos) {
	digitos [1] = numero - numero % 10 * 10 + 48;

	digitos [0] = numero % 10 + 48;
}

void main2 () {
	char *tamaniomensajecasteado = malloc (2);
	char *caracter = malloc (1);
	char *mensaje = malloc (1);
	int tamaniomensaje = 0;

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	int cliente = socket (AF_INET, SOCK_STREAM, 0);

	if (connect (cliente, (void*) &direccionservidor, sizeof (direccionservidor)) != 0) {
		perror ("No me pude conectar.\n");

		exit (EXIT_FAILURE);
	}

	printf ("Estoy aquí.\nMensaje para el servidor: ");

	*caracter = getchar ();

	while (*caracter != '\n') {
		while (*caracter != '\n') {
			mensaje = realloc (mensaje, tamaniomensaje + 1);

			mensaje [tamaniomensaje] = *caracter;

			tamaniomensaje++;

			*caracter = getchar ();
		}

		castear (tamaniomensaje, tamaniomensajecasteado);

		send (cliente, tamaniomensajecasteado, 2, 0);

		printf ("Enviar.");

		scanf ("%c", caracter);

		send (cliente, mensaje, tamaniomensaje, 0);

		tamaniomensaje = 0;

		printf ("Mensaje para el servidor: ");

		*caracter = getchar ();
	}

	free (mensaje);

	tamaniomensaje = 20;

	castear (tamaniomensaje, tamaniomensajecasteado);

	send (cliente, tamaniomensajecasteado, 1, 0);

	printf ("Terminar.");

	scanf ("%c", caracter);

	send (cliente, "Adiós servidor ;).\n", 20, 0);

	close (cliente);
}
