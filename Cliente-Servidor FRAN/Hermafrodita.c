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

int escuchar () {
	struct sockaddr_in direccioncliente;
	char *mensaje = malloc (1);
	int tamaniomensaje;
	fd_set temporal;
	fd_set general;
	int maximofd;
	int nuevofd;

	FD_ZERO (&general);

	FD_ZERO (&temporal);

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	int servidor = socket (AF_INET, SOCK_STREAM, 0);

	int activado = 1;

	setsockopt (servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof (activado));

	if (bind (servidor, (void*) &direccionservidor, sizeof (direccionservidor)) != 0) {
		perror ("Falló el bind.\n");

		return 1;
	}

	printf ("Estoy escuchando.\n");

	listen (servidor, SOMAXCONN);

	FD_SET (servidor, &general);

	maximofd = servidor;

	while (1) {
		temporal = general;

		if (select (maximofd + 1, &temporal, NULL, NULL, NULL) == -1) {
			perror ("Falló el select.\n");

			return 1;
		}

		for (int i = 0; i <= maximofd; i++) {
			if (FD_ISSET (i, &temporal)) {
				if (i == servidor) {
					int tamaniodireccion = sizeof (struct sockaddr_in);

					if ((nuevofd = accept (servidor, (void*) &direccioncliente, &tamaniodireccion)) == -1)
						perror ("Ocurrió un error.\n");

					else {
						FD_SET (nuevofd, &general);

						if (nuevofd > maximofd)
							maximofd = nuevofd;

						printf ("Nueva conexión en %i.\n", nuevofd);
					}
				}

				else {
					mensaje = realloc (mensaje, 1);

					if (recv (i, mensaje, 1, 0) <= 0) {
						printf ("El cliente %i se desconectó.\n", i);

						close (i);

						FD_CLR (i, &general);
					}

					else {
						tamaniomensaje = *mensaje;

						mensaje = realloc (mensaje, tamaniomensaje + 1);

						mensaje [tamaniomensaje] = '\0';

						if (recv (i, mensaje, tamaniomensaje, 0) <= 0) {
							printf ("El cliente %i se desconectó.\n", i);

							close (i);

							FD_CLR (i, &general);
						}

						else {
							printf ("%s\n", mensaje);

							char *aux = strdup (mensaje);

							sprintf (mensaje, "%c%s", strlen (aux), aux);

							free (aux);

							mensaje [tamaniomensaje + 1] = '\0';

							for (int j = 0; j <= maximofd; j++) {
								if (FD_ISSET (j, &general)) {
									if (j != servidor) {
										if (send (j, mensaje, tamaniomensaje + 1, 0) == -1)
											perror ("Error al enviar un mensaje.\n");
									}
								}
							}
						}
					}
				}
			}
		}
	}

	free (mensaje);

	close (servidor);

	return 0;
}

int hablar () {
	char *tamaniomensajecasteado = malloc (1);
	char *caracter = malloc (1);
	char *mensaje = malloc (1);
	int tamaniomensaje = 0;

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	int cliente = socket (AF_INET, SOCK_STREAM, 0);

	int servidor;

	if ((servidor = connect (cliente, (void*) &direccionservidor, sizeof (direccionservidor))) != 0) {
		perror ("No me pude conectar.\n");

		return 1;
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

		mensaje [tamaniomensaje] = '\0';

		char *aux = strdup (mensaje);

		sprintf (mensaje, "%c%s", strlen (aux), aux);

		free (aux);

		send (cliente, mensaje, tamaniomensaje + 1, 0);

		tamaniomensaje = 0;

		recv (cliente, mensaje, 1, 0);

		tamaniomensaje = *mensaje;

		recv (cliente, mensaje, tamaniomensaje + 1, 0);

		mensaje [tamaniomensaje] = '\0';

		printf ("%s\n", mensaje);

		tamaniomensaje = 0;

		printf ("Mensaje para el servidor: ");

		*caracter = getchar ();
	}

	free (mensaje);

	tamaniomensaje = 20;

	sprintf (mensaje, "%c%s", 20, "Adiós servidor ;).\n");

	mensaje [20] = '\0';

	send (cliente, mensaje, 20, 0);

	free (mensaje);

	close (cliente);

	return 0;
}

int main () {
	char *comando = malloc (10);
	char *basura = malloc (1);

	while (1) {
		printf ("####### COMANDOS #######");

		printf ("\n\n");

		printf ("escuchar");

		printf ("\n");

		printf ("hablar");

		printf ("\n");

		printf ("terminar");

		printf ("\n\n");

		printf ("Comando: ");

		scanf ("%s", comando);

		scanf ("%c", basura);

		if (! strcmp (comando, "escuchar"))
			escuchar ();
		else if (! strcmp (comando, "hablar"))
			hablar ();
		else if (! strcmp (comando, "terminar"))
			break;
		else
			printf ("Comando inválido.\n");
	}

	free (comando);

	free (basura);

	return 0;
}
