#ifndef SAC_CLI_HABLAR_SERVIDOR_H_
#define SAC_CLI_HABLAR_SERVIDOR_H_

#include <commons/config.h>
#include <readline/readline.h>
#include <commons/string.h>
#include <commons/log.h>  //ver si es necesario
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

t_config* config;
char ip_server[20];
int puerto_server;
void datos_server();
char* pedir_oper_sacServer(const char* pedido_oper, int codigo_oper);

#endif /* SAC_CLI_HABLAR_SERVIDOR_H_ */
