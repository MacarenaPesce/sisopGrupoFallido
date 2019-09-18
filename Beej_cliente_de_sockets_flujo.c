
    /*
    ** client.c -- Ejemplo de cliente de sockets de flujo
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <string.h>
    #include <netdb.h>
    #include <sys/types.h>
    #include <netinet/in.h>
    #include <sys/socket.h>

    #define PORT 3490 // puerto al que vamos a conectar

    #define MAXDATASIZE 100 // máximo número de bytes que se pueden leer de una vez 

    int main()
    {
    	int numbytes;
    	int socket_cliente;
    	char buf[MAXDATASIZE];
		struct sockaddr_in server_addr; // información de la dirección de destino


    	memset(&(server_addr.sin_zero), '\0', 8);  // poner a cero el resto de la estructura
    	server_addr.sin_family = AF_INET;    // Ordenación de bytes de la máquina
    	server_addr.sin_port = htons(PORT);  // short, Ordenación de bytes de la red
    	server_addr.sin_addr.s_addr = INADDR_ANY;//*((struct in_addr *)he->h_addr);


        socket_cliente = socket(AF_INET, SOCK_STREAM, 0)


        if (socket_cliente == -1) {
            perror("socket");
            exit(1);
        }


        if (connect(socket_cliente, (struct sockaddr *)&server_addr,sizeof(struct sockaddr)) == -1) {
            perror("connect");
            exit(1);
        }

        if ((numbytes=recv(socket_cliente, buf, MAXDATASIZE-1, 0)) == -1) {
            perror("recv");
            exit(1);
        }

        buf[numbytes] = '\0';

        printf("Received: %s",buf);

        close(socket_cliente);

        return 0;
    } 
