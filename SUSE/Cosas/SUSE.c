#include "SUSE.h"

void suse_init () {
	char ***vals = config_get_array_value (config, "Vals");
	char ***maxs = config_get_array_value (config, "Maxs");
	char ***sids = config_get_array_value (config, "Ids");

	pthread_mutex_lock (&logs);

	log_info (log, "Inicializando semáforos.");

	pthread_mutex_unlock (&logs);

	for (int i = 0; i < sizeof (config_get_array_value (config, "Vals")) - 1; i++) {
		struct semaforo *semaforo = malloc (sizeof (struct semaforo));

		semaforo -> sig = semaforos;

		semaforo -> sid = *sids [i];

		semaforo -> max = atoi (maxs [i]);

		semaforo -> val = atoi (vals [i]);

		semaforos = semaforo;
	}

	pthread_mutex_lock (&logs);

	log_info (log, "Semáforos iniciados con éxito.");

	pthread_mutex_unlock (&logs);
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

void crear (void *argumentos) {
	struct parametros *parametros = malloc (sizeof (struct parametros));
	struct ult *ult = malloc (sizeof (struct ult));

	parametros = (struct paramentros*) argumentos;

	printf ("Creando.\n");

	pthread_mutex_lock (parametros -> programa -> mutex);

	pthread_mutex_lock (&logs);

	log_info (log, "Creando un ult para el programa %i.", parametros -> programa -> pid);

	pthread_mutex_unlock (&logs);

	ult -> tid = (parametros -> request [1] - 48) * 1000 + (parametros -> request [2] - 48) * 100 + (parametros -> request [3] - 48) * 10 + parametros -> request [4] - 48;

	ult -> estimacion = 0;

	ult -> duracion = 0;

	ult -> estado = Listo;

	ult -> joineados = NULL;

	ult -> sig = parametros -> programa -> ults;

	parametros -> programa -> ults = ult;

	pthread_mutex_lock (&logs);

	log_info (log, "El ult será el %i.", ult -> tid);

	pthread_mutex_unlock (&logs);

	if (! parametros -> programa -> tienehilos) {
		pthread_mutex_unlock (parametros -> programa -> hilos);

		parametros -> programa -> tienehilos = 1;
	}

	pthread_mutex_unlock (parametros -> programa -> mutex);

	free (parametros -> atendedor);

	free (parametros -> request);

	free (parametros);
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

	printf ("Planificando.\n");

	if (! programa -> tienehilos)
		pthread_mutex_lock (programa -> hilos);

	pthread_mutex_lock (programa -> mutex);

	pthread_mutex_lock (&logs);

	log_info (log, "Planificando el siguente ult del programa %i.", programa -> pid);

	pthread_mutex_unlock (&logs);

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

			pthread_mutex_lock (&logs);

			log_info (log, "El estimado del ult %i es %f.", recorrido -> tid, estimado);

			pthread_mutex_unlock (&logs);

			if (estimado < mejor) {
				mejor = estimado;

				tid = recorrido -> tid;
			}
		}

		recorrido = recorrido -> sig;
	}

	pthread_mutex_lock (&logs);

	log_info (log, "El que sigue es el ult %i con el estimado %f.", tid, mejor);

	pthread_mutex_unlock (&logs);

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

	pthread_mutex_unlock (programa -> mutex);
}

void joinear (void *argumentos) {
	struct parametros *parametros = malloc (sizeof (struct parametros));
	struct ult *recorrido = parametros -> programa -> ults;
	struct ult *joineado;
	int tid;

	parametros = (struct paramentros*) argumentos;

	printf ("Joineando.\n");

	pthread_mutex_lock (parametros -> programa -> mutex);

	while (recorrido) {
		if (recorrido -> estado == Ejecutando || recorrido -> tid == 0) {
			recorrido -> estado = Bloqueado;

			recorrido -> duracion = (int) clock () - recorrido -> inicio;

			joineado = recorrido;

			break;
		}

		recorrido = recorrido -> sig;
	}

	tid = (parametros -> request [1] - 48) * 1000 + (parametros -> request [2] - 48) * 100 + (parametros -> request [3] - 48) * 10 + parametros -> request [4] - 48;

	pthread_mutex_lock (&logs);

	log_info (log, "Joineando el ult %i con el ult %i.", recorrido -> tid, tid);

	pthread_mutex_unlock (&logs);

	recorrido = parametros -> programa -> ults;

	while (recorrido) {
		if (recorrido -> tid == tid) {
			joineado -> sig = recorrido -> joineados;

			recorrido -> joineados = joineado;

			break;
		}

		recorrido = recorrido -> sig;
	}

	if (! recorrido) {
		recorrido = parametros -> programa -> ults;

		while (recorrido -> tid != joineado -> tid)
			recorrido = recorrido -> sig;

		recorrido -> estado = Listo;
	}

	pthread_mutex_unlock (parametros -> programa -> mutex);

	free (parametros -> atendedor);

	free (parametros -> request);

	free (parametros);
}

void cerrar (void *argumentos) {
	struct parametros *parametros = malloc (sizeof (struct parametros));
	struct ult *ult = parametros -> programa -> ults;
	struct ult *recorrido;
	struct ult *joineado;
	int tid;

	parametros = (struct paramentros*) argumentos;

	printf ("Cerrando.\n");

	pthread_mutex_lock (parametros -> programa -> mutex);

	tid = (parametros -> request [1] - 48) * 1000 + (parametros -> request [2] - 48) * 100 + (parametros -> request [3] - 48) * 10 + parametros -> request [4] - 48;

	pthread_mutex_lock (&logs);

	log_info (log, "Cerrando el ult %i.", tid);

	pthread_mutex_unlock (&logs);

	while (ult -> tid != tid)
		ult = ult -> sig;

	joineado = ult -> joineados;

	while (joineado) {
		joineado -> estado = Listo;

		pthread_mutex_lock (&logs);

		log_info (log, "Liberando el ult joineado %i.", joineado -> tid);

		pthread_mutex_unlock (&logs);

		joineado = joineado -> sig;
	}

	recorrido = parametros -> programa -> ults;

	if (recorrido -> tid == tid)
		parametros -> programa -> ults = ult -> sig;

	else {
		while (recorrido -> sig -> tid != tid)
			recorrido = recorrido -> sig;

		recorrido -> sig = ult -> sig;
	}

	pthread_mutex_lock (&logs);

	log_info (log, "Liberando la memoria del ult %i.", tid);

	pthread_mutex_unlock (&logs);

	pthread_mutex_unlock (parametros -> programa -> mutex);

	free (ult);

	free (parametros -> atendedor);

	free (parametros -> request);

	free (parametros);
}

void atender (struct parametros *argumentos) {
	struct parametros *parametros = malloc (sizeof (struct parametros));

	pthread_mutex_lock (&vector);

	parametros -> request = strdup (argumentos -> request);

	parametros -> cliente = argumentos -> cliente;

	pthread_mutex_unlock (&vector);

	parametros -> programa = buscarprograma (parametros -> cliente);

	parametros -> atendedor = malloc (sizeof (pthread_t));

	pthread_mutex_lock (&logs);

	log_info (log, "Atendiendo al cliente %i.", parametros -> cliente);

	pthread_mutex_unlock (&logs);

	switch (parametros -> request [0]) {
		case 1: pthread_create (parametros -> atendedor, NULL, &crear, (void *) parametros); break;
		case 2: planificar (parametros -> programa, parametros -> cliente); break;
		case 3: pthread_create (parametros -> atendedor, NULL, &joinear, (void *) parametros); break;
		case 4: pthread_create (parametros -> atendedor, NULL, &cerrar, (void *) parametros); break;
	}
}

void despertar () {
	struct parametros *parametros = malloc (sizeof (struct parametros));
	struct sockaddr_in direccioncliente;
	char *mensaje = malloc (1);
	struct programa *recorrido;
	struct programa *programa;
	int tamaniomensaje;
	struct ult *ult;
	struct ult *aux;
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

	pthread_mutex_lock (&logs);

	log_info (log, "Levantando servidor.");

	pthread_mutex_unlock (&logs);

	int servidor = socket (AF_INET, SOCK_STREAM, 0);

	int activado = 1;

	pthread_mutex_lock (&logs);

	log_info (log, "Servidor levantado en el fichero %i.", servidor);

	pthread_mutex_unlock (&logs);

	setsockopt (servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof (activado));

	if (bind (servidor, (void*) &direccionservidor, sizeof (direccionservidor)) != 0) {
		perror ("Falló el bind.\n");

		log_info (log, "\"Address already in use\" re cualquiera je :}.");

		exit (EXIT_FAILURE);
	}

	pthread_mutex_lock (&logs);

	log_info (log, "Estoy escuchando.");

	pthread_mutex_unlock (&logs);

	listen (servidor, SOMAXCONN);

	FD_SET (servidor, &general);

	maximofd = servidor;

	while (1) {
		temporal = general;

		if (select (maximofd + 1, &temporal, NULL, NULL, NULL) == -1) {
			perror ("Falló el select.\n");

			pthread_mutex_lock (&logs);

			log_info (log, "Falló el select ]/.");

			pthread_mutex_unlock (&logs);

			exit (EXIT_FAILURE);
		}

		for (int i = 0; i <= maximofd; i++) {
			if (FD_ISSET (i, &temporal)) {
				if (i == servidor) {
					int tamaniodireccion = sizeof (struct sockaddr_in);

					if ((nuevofd = accept (servidor, (void*) &direccioncliente, &tamaniodireccion)) == -1) {
						perror ("Ocurrió un error.\n");

						pthread_mutex_lock (&logs);

						log_info (log, "Falló la conexión a un nuevo cliente.");

						pthread_mutex_unlock (&logs);
					}

					else {
						FD_SET (nuevofd, &general);

						if (nuevofd > maximofd)
							maximofd = nuevofd;

						pthread_mutex_lock (&logs);

						log_info (log, "Nueva conexión en %i.", nuevofd);

						pthread_mutex_unlock (&logs);

						programa = malloc (sizeof (struct programa));

						programa -> pid = nuevofd;

						programa -> ults = NULL;

						programa -> tienehilos = 0;

						programa -> mutex = malloc (sizeof (pthread_mutex_t));

						pthread_mutex_init (programa -> mutex, NULL);

						programa -> hilos = malloc (sizeof (pthread_mutex_t));

						pthread_mutex_init (programa -> hilos, NULL);

						pthread_mutex_lock (programa -> hilos);

						programa -> sig = programas;

						programas = programa;
					}
				}

				else {
					mensaje = realloc (mensaje, 1);

					if (recv (i, mensaje, 1, 0) <= 0) {
						programa = buscarprograma (i);
						recorrido = programas;
						ult = programa -> ults;

						pthread_mutex_lock (&logs);

						log_info (log, "El cliente %i se desconectó.", i);

						pthread_mutex_unlock (&logs);

						while (ult) {
							aux = ult -> sig;

							pthread_mutex_lock (&logs);

							log_info (log, "Se liberó la memoria del ult %i del programa %i.", ult -> tid, i);

							pthread_mutex_unlock (&logs);

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

						pthread_mutex_lock (&logs);

						log_info (log, "Se liberó la memoria del programa %i.", i);

						pthread_mutex_unlock (&logs);

						free (programa -> mutex);

						free (programa -> hilos);

						free (programa);

						close (i);

						FD_CLR (i, &general);
					}

					else {
						tamaniomensaje = *mensaje;

						mensaje = realloc (mensaje, tamaniomensaje + 1);

						mensaje [tamaniomensaje] = '\0';

						recv (i, mensaje, tamaniomensaje, 0);

						mensaje [tamaniomensaje + 1] = '\0';

						pthread_mutex_lock (&vector);

						parametros -> request = mensaje;

						parametros -> cliente = i;

						pthread_mutex_unlock (&vector);

						atender (parametros);
					}
				}
			}
		}
	}

	free (mensaje);

	free (parametros);

	close (servidor);

	exit (EXIT_SUCCESS);
}

void main () {
	config = config_create ("Cosas/Config.config");

	log = log_create ("Log.log", "SUSE.c", 1, LOG_LEVEL_INFO);

	pthread_mutex_init (&logs, NULL);

	pthread_mutex_init (&vector, NULL);

	suse_init ();

	despertar ();
}
