#ifndef SAC_SERVER_ESCUCHAR_PEDIDOS_H_
#define SAC_SERVER_ESCUCHAR_PEDIDOS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <commons/string.h>
#include <pthread.h>

#define MYPORT 3980    // Puerto al que conectarán los usuarios

#define DESCRIBIR_DIRECTORIO 23
#define ELIMINAR_DIRECTORIO 22
#define LISTAR_DIRECTORIO 21
#define CREAR_DIRECTORIO 20

#define DESCRIBIR_ARCHIVO 7
#define ELIMINAR_ARCHIVO 6
#define LEER_ARCHIVO 5
#define ESCRIBIR_ARCHIVO 4
#define CREAR_ARCHIVO 3

struct cliente_op {
	int new_fd;
	struct sockaddr_in their_addr;
};


int largo_archivo(FILE* archivo);
char* sac_leer(char* path);
void control_error_conexion(int bytes_leidos, struct sockaddr_in cliente,
		int fd_cliente);
void* atender_pedidos(void* cliente_nuevo);

#endif /* SAC_SERVER_ESCUCHAR_PEDIDOS_H_ */
