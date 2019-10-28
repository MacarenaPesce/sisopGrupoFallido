#include <commons/config.h>
#include <commons/log.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#ifndef SUSE_H_
#define SUSE_H_

pthread_mutex_t vector;

pthread_mutex_t logs;

t_config *config;

t_log *log;

struct semaforo *semaforos = NULL;

struct programa *programas = NULL;

struct semaforo {
	struct semaforo *sig;
	char sid;
	int max;
	int val;
};

struct programa {
	pthread_mutex_t *hilos;
	pthread_mutex_t *mutex;
	struct programa *sig;
	struct ult *ults;
	char tienehilos;
	int pid;
};

struct ult {
	struct ult *joineados;
	double estimacion;
	struct ult *sig;
	int duracion;
	int inicio;
	int estado;
	int tid;
};

struct parametros {
	struct programa* programa;
	pthread_t *atendedor;
	char *request;
	int cliente;
};

enum estado {
	Listo,
	Ejecutando,
	Bloqueado
};

#endif
