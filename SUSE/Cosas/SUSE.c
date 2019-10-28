#include "SUSE.h"

void suse_init () {
	char ***vals = config_get_array_value (config, "Vals");
	char ***maxs = config_get_array_value (config, "Maxs");
	char ***sids = config_get_array_value (config, "Ids");

	log_info (log, "Inicializando semáforos.");

	for (int i = 0; i < sizeof (config_get_array_value (config, "Vals")) - 1; i++) {
		struct semaforo *semaforo = malloc (sizeof (struct semaforo));

		semaforo -> sig = semaforos;

		semaforo -> sid = *sids [i];

		semaforo -> max = atoi (maxs [i]);

		semaforo -> val = atoi (vals [i]);

		semaforos = semaforo;
	}

	log_info (log, "Semáforos iniciados con éxito.");
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

void crear (struct programa *programa, char *request) {
	struct ult *ult = malloc (sizeof (struct ult));

	log_info (log, "Creando un ult para el programa %i.", programa -> pid);

	ult -> tid = (request [1] - 48) * 1000 + (request [2] - 48) * 100 + (request [3] - 48) * 10 + request [4] - 48;

	ult -> estimacion = 0;

	ult -> duracion = 0;

	ult -> estado = Listo;

	ult -> joineados = NULL;

	ult -> sig = programa -> ults;

	programa -> ults = ult;

	log_info (log, "El ult será el %i.", ult -> tid);
}

double estimacion (double estimacion, int duracion) {
	return estimacion * config_get_double_value (config, "Alfa") + duracion * (1 - config_get_double_value (config, "Alfa"));
}

void planificar (struct programa *programa, int cliente) {
	struct ult *recorrido = programa -> ults;
	char *tidcasteado = malloc (5);
	double estimado;
	int mejor;
	int tid;

	log_info (log, "Planificando el siguente ult del programa %i.", programa -> pid);

	while (recorrido) {
		if (recorrido -> estado == Ejecutando) {
			recorrido -> estado = Listo;

			recorrido -> duracion = (int) clock () - recorrido -> inicio;
		}

		recorrido = recorrido -> sig;
	}

	recorrido = programa -> ults;

	while (recorrido) {
		if (recorrido -> estado == Listo) {
			estimado = estimacion (recorrido -> estimacion, recorrido -> duracion);

			mejor = estimado;

			tid = recorrido -> tid;

			break;
		}

		recorrido = recorrido -> sig;
	}

	recorrido = programa -> ults;

	while (recorrido) {
		if (recorrido -> estado == Listo) {
			estimado = estimacion (recorrido -> estimacion, recorrido -> duracion);

			log_info (log, "El estimado del ult %i es %f.", recorrido -> tid, estimado);

			if (estimado < mejor) {
				mejor = estimado;

				tid = recorrido -> tid;
			}
		}

		recorrido = recorrido -> sig;
	}

	log_info (log, "El que sigue es el ult %i con el estimado %f.", tid, mejor);

	recorrido = programa -> ults;

	while (recorrido -> tid != tid)
		recorrido = recorrido -> sig;

	tidcasteado [0] = tid / 1000 + 48;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10 + 48;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10 + 48;

	tidcasteado [3] = tid - (tid / 10) * 10 + 48;

	tidcasteado [4] = '\0';

	recorrido -> estimacion = mejor;

	recorrido -> inicio = (int) clock ();

	recorrido -> estado = Ejecutando;

	send (cliente, tidcasteado, 5, 0);

	free (tidcasteado);
}

void joinear (struct programa *programa, char *request) {
	struct ult *recorrido = programa -> ults;
	struct ult *joineado;
	int tid;

	while (recorrido) {
		if (recorrido -> estado == Ejecutando || recorrido -> tid == 0) {
			recorrido -> estado = Bloqueado;

			recorrido -> duracion = (int) clock () - recorrido -> inicio;

			joineado = recorrido;

			break;
		}

		recorrido = recorrido -> sig;
	}

	tid = (request [1] - 48) * 1000 + (request [2] - 48) * 100 + (request [3] - 48) * 10 + request [4] - 48;

	log_info (log, "Joineando el ult %i con el ult %i.", recorrido -> tid, tid);

	recorrido = programa -> ults;

	while (recorrido) {
		if (recorrido -> tid == tid) {
			joineado -> sig = recorrido -> joineados;

			recorrido -> joineados = joineado;

			break;
		}

		recorrido = recorrido -> sig;
	}

	if (! recorrido) {
		recorrido = programa -> ults;

		while (recorrido -> tid != joineado -> tid)
			recorrido = recorrido -> sig;

		recorrido -> estado = Listo;
	}
}

void cerrar (struct programa *programa, char *request) {
	struct ult *ult = programa -> ults;
	struct ult *recorrido;
	struct ult *joineado;
	int tid;

	tid = (request [1] - 48) * 1000 + (request [2] - 48) * 100 + (request [3] - 48) * 10 + request [4] - 48;

	log_info (log, "Cerrando el ult %i.", tid);

	while (ult -> tid != tid)
		ult = ult -> sig;

	joineado = ult -> joineados;

	while (joineado) {
		joineado -> estado = Listo;

		log_info (log, "Liberando el ult joineado %i.", joineado -> tid);

		joineado = joineado -> sig;
	}

	recorrido = programa -> ults;

	if (recorrido -> tid == tid)
		programa -> ults = ult -> sig;

	else {
		while (recorrido -> sig -> tid != tid)
			recorrido = recorrido -> sig;

		recorrido -> sig = ult -> sig;
	}

	log_info (log, "Liberando la memoria del ult %i.", tid);

	free (ult);
}

void atender (int cliente, char *request) {
	log_info (log, "Atendiendo al cliente %i.", cliente);

	pthread_mutex_lock (&mutex);

	switch (*request) {
		case 1: crear (buscarprograma (cliente), request); break;
		case 2: planificar (buscarprograma (cliente), cliente); break;
		case 3: joinear (buscarprograma (cliente), request); break;
		case 4: cerrar (buscarprograma (cliente), request); break;
	}

	pthread_mutex_unlock (&mutex);
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

	log_info (log, "Levantando servidor.");

	int servidor = socket (AF_INET, SOCK_STREAM, 0);

	int activado = 1;

	log_info (log, "Servidor levantado en el fichero %i.", servidor);

	setsockopt (servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof (activado));

	if (bind (servidor, (void*) &direccionservidor, sizeof (direccionservidor)) != 0) {
		perror ("Falló el bind.\n");

		log_info (log, "\"Address already in use\" re cualquiera je :}.");

		exit (EXIT_FAILURE);
	}

	log_info (log, "Estoy escuchando.");

	listen (servidor, SOMAXCONN);

	FD_SET (servidor, &general);

	maximofd = servidor;

	while (1) {
		temporal = general;

		if (select (maximofd + 1, &temporal, NULL, NULL, NULL) == -1) {
			perror ("Falló el select.\n");

			log_info (log, "Falló el select ]/.");

			exit (EXIT_FAILURE);
		}

		for (int i = 0; i <= maximofd; i++) {
			if (FD_ISSET (i, &temporal)) {
				if (i == servidor) {
					int tamaniodireccion = sizeof (struct sockaddr_in);

					if ((nuevofd = accept (servidor, (void*) &direccioncliente, &tamaniodireccion)) == -1) {
						perror ("Ocurrió un error.\n");

						log_info (log, "Falló la conexión a un nuevo cliente.");
					}

					else {
						FD_SET (nuevofd, &general);

						if (nuevofd > maximofd)
							maximofd = nuevofd;

						log_info (log, "Nueva conexión en %i.", nuevofd);

						struct programa *programa = malloc (sizeof (struct programa));

						programa -> pid = nuevofd;

						programa -> ults = NULL;

						programa -> sig = programas;

						programas = programa;
					}
				}

				else {
					mensaje = realloc (mensaje, 1);

					if (recv (i, mensaje, 1, 0) <= 0) {
						struct programa *programa = buscarprograma (i);
						struct programa *recorrido = programas;
						struct ult *ult = programa -> ults;

						log_info (log, "El cliente %i se desconectó.", i);

						while (ult) {
							struct ult *aux = ult -> sig;

							log_info (log, "Se liberó la memoria del ult %i del programa %i.", ult -> tid, i);

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

						log_info (log, "Se liberó la memoria del programa %i.", i);

						free (programa);

						close (i);

						FD_CLR (i, &general);
					}

					else {
						tamaniomensaje = *mensaje;

						mensaje = realloc (mensaje, tamaniomensaje + 1);

						mensaje [tamaniomensaje] = '\0';

						pthread_mutex_lock (&mutex);

						recv (i, mensaje, tamaniomensaje, 0);

						pthread_mutex_unlock (&mutex);

						mensaje [tamaniomensaje + 1] = '\0';

						atender (i, mensaje);
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
