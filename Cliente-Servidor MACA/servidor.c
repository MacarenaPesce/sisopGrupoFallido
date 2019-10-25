#include <hilolay/hilolay.h>
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

void main () {
	//Usando Hilolay
	int tiempo = lib_get_time();
	printf("El tiempo es %i\n", tiempo);


	struct sockaddr_in direccioncliente;
	char *mensaje = malloc (1);
	int tamaniomensaje;

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	int servidor = socket (AF_INET, SOCK_STREAM, 0);

	int activado = 1;

	setsockopt (servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof (activado));

	if (bind (servidor, (void*) &direccionservidor, sizeof (direccionservidor)) != 0) {
		perror ("Falló el bind.\n");

		exit (EXIT_FAILURE);
	}

	printf ("Estoy escuchando.\n");

	listen (servidor, SOMAXCONN);

	int tamaniodireccion = sizeof (struct sockaddr_in);

	int cliente = accept (servidor, (void*) &direccioncliente, &tamaniodireccion);

	if (cliente == -1) {
		perror ("Ocurrió un error.\n");

		exit (EXIT_FAILURE);
	}

	printf ("Nueva conexión en %i.\n", cliente);

	while (-1) {
		mensaje = realloc (mensaje, 2);

		recv (cliente, mensaje, 2, 0);

		tamaniomensaje = atoi (mensaje);

		mensaje = realloc (mensaje, tamaniomensaje);

		mensaje [tamaniomensaje] = '\0';

		recv (cliente, mensaje, tamaniomensaje, 0);

		printf ("%s\n", mensaje);
	}

	free (mensaje);

	close (servidor);
}
