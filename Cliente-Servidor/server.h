#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<unistd.h>
#include<commons/collections/list.h>
//#include<sys/types.h>   tipos de datos   https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html
//#include<sys/socket.h>  encabezado de enchufes principales  https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/socket.h.html
//#include<netdb.h>   definiciones para operaciones de bases de datos de red   https://pubs.opengroup.org/onlinepubs/007908799/xns/netdb.h.html
//#include<unistd.h>  constantes y tipos simbólicos estándar https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html
//#include<commons/collections/list.h>


#ifndef SERVER_H
#define SERVER_H

#define BACKLOG SOMAXCONN  // Define cuantas conexiones vamos a mantener pendientes al mismo tiempo
#define PACKAGESIZE 1024   // Define cual va a ser el size maximo del paquete a enviar

int createServer(char*, char*);
int connectToClient(int);
int readQueryFromClient(int, char*);
void closeServer(int);

#endif
