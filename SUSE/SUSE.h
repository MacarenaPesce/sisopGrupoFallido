#include "hilolay/hilolay.h"
#include <commons/config.h>
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
