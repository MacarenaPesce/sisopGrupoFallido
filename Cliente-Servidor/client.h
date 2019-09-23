#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>



#ifndef CLIENT_H
#define CLIENT_H

#define PACKAGESIZE 1024  // Define cual va a ser el size maximo del paquete a enviar

int connectToServer();
void sendMessage(int, char*);
void sendMessages(int);
void closeConnection(int);

#endif



// buscar donde esta struct addrinfo -> en los include -> https://linux.die.net/man/3/freeaddrinfo
//struct addrinfo {
//    int ai_flags;
//    int ai_family;
//    int ai_socktype;
//    int ai_protocol;
//    socklen_t ai_addrlen;
//    struct sockaddr * ai_addr;
//    char * ai_canonname;
//    struct addrinfo * ai_next;
//};
//memset Establece el primer num bytes del bloque de memoria apuntado por ptr a la especificada valor (interpretado como un unsigned char ).
//Las funciones getaddrinfo () y getnameinfo () convierten nombres de dominio , nombres de host y direcciones IP entre representaciones de texto legibles por humanos y formatos binarios estructurados para la API de red del sistema operativo . Ambas funciones están contenidas en la interfaz de programación de aplicaciones (API) estándar POSIX .
// getaddrinfo, freeaddrinfo, gai_strerror - dirección de red y traducción de servicios
// https://linux.die.net/man/3/freeaddrinfo