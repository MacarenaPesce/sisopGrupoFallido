#include "hilolay/hilolay_internal.h"
#include "hilolay/hilolay_alumnos.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

int cantults = 0;

int cliente;

int suse_create (int tid) {
	char *tidcasteado;
	char *mensaje;

	if (cantults + 1 > MAX_ULTS)
		return ERROR_TOO_MANY_ULTS;

	cantults++;

	tidcasteado [0] = tid / 1000;

	tidcasteado [1] = tid / 100 - (tid / 1000) * 10;

	tidcasteado [2] = tid / 10 - (tid / 100) * 10;

	tidcasteado [3] = tid - (tid / 10) * 10;

	sprintf (mensaje, "%c%s", 1, tidcasteado);

	send (cliente, mensaje, 5, 0);

	return 0;
}

int suse_schedule_next () {
	return 0;
}

int suse_join (int tid) {
	return 0;
}

int suse_close (int tid) {
	return 0;
}

void hilolay_init () {
	init_internal (main_ops);

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY;
	direccionservidor.sin_port = htons (1024);

	cliente = socket (AF_INET, SOCK_STREAM, 0);

	int servidor;

	if ((servidor = connect (cliente, (void*) &direccionservidor, sizeof (direccionservidor))) != 0) {
		perror ("No me pude conectar.\n");

		exit (EXIT_FAILURE);
	}
}
