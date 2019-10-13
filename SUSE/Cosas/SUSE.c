#include "SUSE.h"

void _suse_init () {
	t_config *config = config_create ("Cosas/Config.config");
	char ***vals = config_get_array_value (config, "Vals");
	char ***maxs = config_get_array_value (config, "Maxs");
	char ***ids = config_get_array_value (config, "Ids");

	char id;

	int val;

	int max;

	for (int i = 0; i < sizeof (config_get_array_value (config, "Vals")) - 1; i++) {
		struct semaforo *sem = malloc (sizeof (struct semaforo));

		sem -> sig = repo;

		id = *ids [i];

		sem -> id = id;

		max = atoi (maxs [i]);

		sem -> max = max;

		val = atoi (vals [i]);

		sem -> val = val;

		repo = sem;
	}
}

void crear (int cliente) {

}

void atender (int cliente, char *request) {
	int codigo = *request;

	switch (codigo) {
		case 1: crear (cliente);
	}
}

void despertar () {
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

		exit (EXIT_FAILURE);
	}

	printf ("Estoy escuchando.\n");

	listen (servidor, SOMAXCONN);

	FD_SET (servidor, &general);

	maximofd = servidor;

	while (1) {
		temporal = general;

		if (select (maximofd + 1, &temporal, NULL, NULL, NULL) == -1) {
			perror ("Falló el select.\n");

			exit (EXIT_FAILURE);
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
							mensaje [tamaniomensaje + 1] = '\0';

							atender (i, mensaje);
						}
					}
				}
			}
		}
	}

	free (mensaje);

	close (servidor);

	exit (EXIT_SUCCESS);
}

void main () {
	_suse_init ();

	despertar ();
}
