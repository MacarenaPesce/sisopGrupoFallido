#include "hilolay/hilolay_internal.h"
#include "hilolay/hilolay_alumnos.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

int cantults = 0;

int servidor;

int cliente;

int suse_create (int tid) {
	char *tidcasteado = malloc (4);
	char *mensaje = malloc (5);

	printf ("3\n");

	if (cantults + 1 > MAX_ULTS)
		return ERROR_TOO_MANY_ULTS;

	cantults++;

	tidcasteado [0] = tid / 1000;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10;

	tidcasteado [3] = tid - (tid / 10) * 10;

	sprintf (mensaje, "%c%s", 1, tidcasteado);

	free (tidcasteado);

	send (cliente, mensaje, 5, 0);

	free (mensaje);

	return 0;
}

int suse_schedule_next () {
	char *mensaje = malloc (1);
	int tid;

	*mensaje = 2;

	send (cliente, mensaje, 1, 0);

	free (mensaje);

	mensaje = malloc (4);

	recv (servidor, mensaje, 4, 0);

	tid = mensaje [0] * 1000 + mensaje [1] * 100 + mensaje [2] * 10 + mensaje [3];

	free (mensaje);

	return tid;
}

int suse_join (int tid) {
	char *tidcasteado = malloc (4);
	char *mensaje = malloc (5);

	tidcasteado [0] = tid / 1000;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10;

	tidcasteado [3] = tid - (tid / 10) * 10;

	sprintf (mensaje, "%c%s", 3, tidcasteado);

	free (tidcasteado);

	send (cliente, mensaje, 5, 0);

	free (mensaje);

	return 0;
}

int suse_close (int tid) {
	char *tidcasteado = malloc (4);
	char *mensaje = malloc (5);

	tidcasteado [0] = tid / 1000;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10;

	tidcasteado [3] = tid - (tid / 10) * 10;

	sprintf (mensaje, "%c%s", 4, tidcasteado);

	free (tidcasteado);

	send (cliente, mensaje, 5, 0);

	free (mensaje);

	cantults--;

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
	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	cliente = socket (AF_INET, SOCK_STREAM, 0);

	if ((servidor = connect (cliente, (void*) &direccionservidor, sizeof (direccionservidor))) != 0) {
		perror ("No me pude conectar.\n");

		exit (EXIT_FAILURE);
	}

	init_internal (&hiloops);
}
