#include "SUSE.h"

void suse_init () {
	char ***vals = config_get_array_value (config, "Vals");
	char ***maxs = config_get_array_value (config, "Maxs");
	char ***sids = config_get_array_value (config, "Ids");

	for (int i = 0; i < sizeof (config_get_array_value (config, "Vals")) - 1; i++) {
		struct semaforo *semaforo = malloc (sizeof (struct semaforo));

		semaforo -> sig = semaforos;

		semaforo -> sid = *sids [i];

		semaforo -> max = atoi (maxs [i]);

		semaforo -> val = atoi (vals [i]);

		semaforos = semaforo;
	}
}

struct programa *buscarprograma (int pid) {
	struct programa *recorrido = programas;

	while (recorrido) {
		if (recorrido -> pid == pid)
			return recorrido;

		recorrido = recorrido -> sig;
	}

	return NULL;
}

void crear (struct programa programa, char *request) {
	struct ult ult;

	ult.tid = request [1] * 1000 + request [2] * 100 + request [3] * 10 + request [4];

	ult.estimacion = 0;

	ult.inicio = (int) clock ();

	ult.estado = Listo;

	ult.sig = programa.ults;

	programa.ults = &ult;
}

double estimacion (double estimacion, int duracion) {
	return estimacion * config_get_double_value (config, "Alfa") + duracion * (1 - config_get_double_value (config, "Alfa"));
}

void planificar (struct programa programa, int cliente) {
	struct ult *recorrido = programa.ults;
	char *tidcasteado = malloc (4);
	double estimado;
	int mejor;
	int tid;

	while (recorrido) {
		if (recorrido -> estado == Listo) {
			estimado = estimacion (recorrido -> estimacion, recorrido -> duracion);

			mejor = estimado;

			tid = recorrido -> tid;

			break;
		}

		recorrido = recorrido -> sig;
	}

	recorrido = programa.ults;

	while (recorrido) {
		if (recorrido -> estado == Listo) {
			estimado = estimacion (recorrido -> estimacion, recorrido -> duracion);

			if (estimado < mejor) {
				mejor = estimado;

				tid = recorrido -> tid;
			}
		}

		recorrido = recorrido -> sig;
	}

	recorrido = programa.ults;

	while (recorrido -> tid != tid)
		recorrido = recorrido -> sig;

	tidcasteado [0] = tid / 1000;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10;

	tidcasteado [3] = tid - (tid / 10) * 10;

	recorrido -> estimacion = mejor;

	recorrido -> inicio = (int) clock ();

	recorrido -> estado = Ejecutando;

	send (cliente, tidcasteado, 4, 0);

	free (tidcasteado);
}

void joinear (struct programa programa, char *request) {
	struct ult *recorrido = programa.ults;
	struct ult *joineado;
	int tid;

	while (recorrido) {
		if (recorrido -> estado == Ejecutando) {
			recorrido -> estado = Bloqueado;

			joineado = recorrido;

			break;
		}

		recorrido = recorrido -> sig;
	}

	tid = request [1] * 1000 + request [2] * 100 + request [3] * 10 + request [4];

	recorrido = programa.ults;

	while (recorrido) {
		if (recorrido -> tid == tid) {
			joineado -> sig = recorrido -> joineados;

			recorrido -> joineados = joineado;

			break;
		}

		recorrido -> sig;
	}
}

void cerrar (struct programa programa, char *request) {
	struct ult *ult = programa.ults;
	struct ult *recorrido;
	struct ult *joineado;
	int tid;

	tid = request [1] * 1000 + request [2] * 100 + request [3] * 10 + request [4];

	while (ult -> tid != tid)
		ult = ult -> sig;

	joineado = ult -> joineados;

	while (joineado) {
		joineado -> estado = Listo;

		joineado = joineado -> sig;
	}

	recorrido = programa.ults;

	if (recorrido -> tid == ult -> tid)
		programa.ults = ult -> sig;

	else {
		while (recorrido -> sig -> tid != ult -> tid)
			recorrido = recorrido -> sig;

		recorrido -> sig = ult -> sig;
	}

	free (ult);
}

void atender (int cliente, char *request) {
	switch (*request) {
		case 1: crear (*buscarprograma (cliente), request); break;
		case 2: planificar (*buscarprograma (cliente), cliente); break;
		case 3: joinear (*buscarprograma (cliente), request); break;
		case 4: cerrar (*buscarprograma (cliente), request); break;
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

						struct programa *programa = malloc (sizeof (struct programa));

						programa -> pid = i;

						programa -> sig = programas;

						programas = programa;
					}
				}

				else {
					mensaje = realloc (mensaje, 1);

					if (recv (i, mensaje, 1, 0) <= 0 || ! buscarprograma (i)) {
						printf ("El cliente %i se desconectó.\n", i);

						if (buscarprograma (i)) {
							struct programa *programa = buscarprograma (i);
							struct programa *recorrido = programas;
							struct ult *ult = programa -> ults;

							while (ult) {
								struct ult *aux = ult -> sig;

								free (ult);

								ult = aux;
							}

							if (recorrido -> pid == programa -> pid)
								programas = programa -> sig;

							else {
								while (recorrido -> sig -> pid != programa -> pid)
									recorrido = recorrido -> sig;

								recorrido -> sig = programa -> sig;
							}

							free (programa);
						}

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
	config = config_create ("Cosas/Config.config");

	log = log_create ("Log.log", "SUSE.c", 1, LOG_LEVEL_INFO);

	suse_init ();

	despertar ();
}
