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

    #define BACKLOG 10     // Cuántas conexiones pendientes se mantienen en cola

struct cliente_op {
	int new_fd;
	struct sockaddr_in their_addr;
};

#endif /* SAC_SERVER_ESCUCHAR_PEDIDOS_H_ */
