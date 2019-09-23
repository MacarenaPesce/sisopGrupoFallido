#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <readline/readline.h>
#include <pthread.h>

#define PORT 3491

    int control_error_conexion(int bytes_leidos, int fd_cliente) {
    	if(bytes_leidos == 0) {
    		printf("Se desconecto el cliente %i\n", fd_cliente);
    		return -1;
    	}
    	if(bytes_leidos < 0) {
    		perror("recv");
    		return -2;
    	}
    }

void* escuchar_server (void* server){
		int *fdServer = (int*) server;
    	int nbytesMSJ;
    	char tamanioMSJ[1];
    	int tamanio;
    	char* mensajeServer;
    	for(;;) {
            	nbytesMSJ = recv(*fdServer, tamanioMSJ, 1, 0);
            	if(control_error_conexion(nbytesMSJ, *fdServer) < 0) {
            		printf("Se perdió la conexión con el server\n");
            		close(*fdServer);
            	} else {
            		tamanio = tamanioMSJ[0];
            		mensajeServer = malloc(tamanio + 1);
            		recv(*fdServer, mensajeServer, strlen(mensajeServer), 0);
            		mensajeServer[tamanio] = '\0';
            		printf("dicen >>> %s\n", mensajeServer);
            		free(mensajeServer);
            }
    	}
    }

int main() {
	int server;
	int numbytes;

    fd_set master;
	struct sockaddr_in their_addr; 

	char tamanioMSJ[2];
	int nbytesMSJ;
	int tamanio;
	char* mensajeServer;

	int max = 255;
	char mensajeParaServer[max + 1];
	char* linea;

	if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET; 
	their_addr.sin_port = htons(PORT);  
	their_addr.sin_addr.s_addr = INADDR_ANY; 
	memset(&(their_addr.sin_zero), '\0', 8); 

	if (connect(server, (struct sockaddr *) &their_addr,sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
	
	pthread_t hilo_escuchar;

	pthread_create(&hilo_escuchar, NULL, escuchar_server, &server);

	while(1) {
		linea = readline("<<< ");
		sprintf(mensajeServer, "%c%s", strlen(linea), linea);
		send(server, mensajeServer, strlen(mensajeServer), 0);
		free(linea);
	}

	pthread_join(hilo_escuchar, NULL);


	return 0;
}
