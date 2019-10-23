#include <commons/config.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef SUSE_H_
#define SUSE_H_

struct semaforo *semaforos = NULL;

struct programa *programas = NULL;

struct semaforo {
	struct semaforo *sig;
	char id;
	int max;
	int val;
};

struct programa {
	struct programa *sig;
	struct ult *ults;
	int id;
};

struct ult {
	struct ult *sig;
	int id;
};

#endif
