#ifndef SAC_CLI_HABLAR_SERVIDOR_H_
#define SAC_CLI_HABLAR_SERVIDOR_H_

#include <commons/config.h>
#include <commons/string.h>
#include <commons/log.h>  //ver si es necesario
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define DESCRIBIR_DIRECTORIO 23
#define ELIMINAR_DIRECTORIO 22
#define LISTAR_DIRECTORIO 21
#define CREAR_DIRECTORIO 20

#define DESCRIBIR_ARCHIVO 7
#define ELIMINAR_ARCHIVO 6
#define LEER_ARCHIVO 5
#define ESCRIBIR_ARCHIVO 4
#define CREAR_ARCHIVO 3

t_config* config;
char ip_server[20];
int puerto_server;
void datos_server();
char* pedir_oper_sacServer(const char* pedido_oper, int codigo_oper);
int sac_leer(const char* path, char* buffer);

#endif /* SAC_CLI_HABLAR_SERVIDOR_H_ */
