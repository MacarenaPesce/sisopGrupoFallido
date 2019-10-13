#include "hilolay/hilolay_internal.h"
#include "hilolay/hilolay.h"
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef SUSE_H_
#define SUSE_H_

struct semaforo *repo = NULL;

struct semaforo {
	struct semaforo *sig;
	char id;
	int max;
	int val;
};

#endif