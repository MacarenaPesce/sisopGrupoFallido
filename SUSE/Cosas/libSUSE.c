#include "hilolay/hilolay_internal.h"
#include "hilolay/hilolay_alumnos.h"
#include <commons/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

int cantults = 0;

int servidor;

int cliente;

t_log *log;

int suse_create (int tid) {
	char *tidcasteado = malloc (5);
	char *mensaje = malloc (7);
	char *aux = malloc (6);

	log_info (log, "Intentando crear un nuevo hilo.");

	if (cantults + 1 > MAX_ULTS) {
		log_info (log, "Abortando creación de hilo, demasiados threads.");

		return ERROR_TOO_MANY_ULTS;
	}

	cantults++;

	log_info (log, "Ahora hay %i threads en el programa %i.", cantults, cliente);

	tidcasteado [0] = tid / 1000 + 48;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10 + 48;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10 + 48;

	tidcasteado [3] = tid - (tid / 10) * 10 + 48;

	tidcasteado [4] = '\0';

	log_info (log, "El tid del nuevo thread es %s.", tidcasteado);

	sprintf (aux, "%c%s", 1, tidcasteado);

	free (tidcasteado);

	sprintf (mensaje, "%c%s", strlen (aux), aux);

	free (aux);

	send (cliente, mensaje, 7, 0);

	free (mensaje);

	return 0;
}

int suse_schedule_next () {
	char *mensaje = malloc (3);
	int tid;

	log_info (log, "Planificando el siguiente hilo.");

	mensaje [0] = 2;

	mensaje [1] = 2;

	mensaje [2] = '\0';

	send (cliente, mensaje, 3, 0);

	free (mensaje);

	mensaje = malloc (5);

	recv (cliente, mensaje, 5, 0);

	tid = (mensaje [0] - 48) * 1000 + (mensaje [1] - 48) * 100 + (mensaje [2] - 48) * 10 + mensaje [3] - 48;

	log_info (log, "El siguiente hilo es el %i.", tid);

	free (mensaje);

	return tid;
}

int suse_join (int tid) {
	char *tidcasteado = malloc (5);
	char *mensaje = malloc (7);
	char *aux = malloc (6);

	log_info (log, "Joineando al hilo %i.", tid);

	tidcasteado [0] = tid / 1000 + 48;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10 + 48;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10 + 48;

	tidcasteado [3] = tid - (tid / 10) * 10 + 48;

	tidcasteado [4] = '\0';

	sprintf (aux, "%c%s", 3, tidcasteado);

	free (tidcasteado);

	sprintf (mensaje, "%c%s", strlen (aux), aux);

	free (aux);

	send (cliente, mensaje, 7, 0);

	free (mensaje);

	return 0;
}

int suse_close (int tid) {
	char *tidcasteado = malloc (5);
	char *mensaje = malloc (7);
	char *aux = malloc (6);

	log_info (log, "Cerrando el hilo %i.", tid);

	tidcasteado [0] = tid / 1000 + 48;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10 + 48;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10 + 48;

	tidcasteado [3] = tid - (tid / 10) * 10 + 48;

	tidcasteado [4] = '\0';

	sprintf (aux, "%c%s", 4, tidcasteado);

	free (tidcasteado);

	sprintf (mensaje, "%c%s", strlen (aux), aux);

	free (aux);

	send (cliente, mensaje, 6, 0);

	free (mensaje);

	cantults--;

	log_info (log, "Ahora hay %i hilos en el programa %i.", cantults, cliente);

	return 0;
}

int suse_wait (int tid, char *sem) {
	return 0;
}

int suse_signal (int tid, char *sem) {
	return 0;
}

static struct hilolay_operations hiloops = {
		.suse_create = &suse_create,
		.suse_schedule_next = &suse_schedule_next,
		.suse_join = &suse_join,
		.suse_close = &suse_close,
		.suse_wait = &suse_wait,
		.suse_signal = &suse_signal
};

void hilolay_init () {
	log = log_create ("Log.log", "libSUSE.c", 1, LOG_LEVEL_INFO);

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	log_info (log, "Creando la conexión con SUSE.");

	cliente = socket (AF_INET, SOCK_STREAM, 0);

	if ((servidor = connect (cliente, (void*) &direccionservidor, sizeof (direccionservidor))) != 0) {
		perror ("No me pude conectar.\n");

		log_info (log, "Error en la conexión, matando proceso...");

		exit (EXIT_FAILURE);
	}

	log_info (log, "Conexión exitosa.");

	init_internal (&hiloops);
}
