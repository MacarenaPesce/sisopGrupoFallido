
	/*
	*
    *  * Servidor multihijos (el que usaría Mari y Noe AK FileSystem)
    * 
    */

    #include <stdio.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <errno.h>
    #include <string.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/wait.h>
    #include <signal.h>

    #define MYPORT 3491    // Puerto al que conectarán los usuarios

    #define BACKLOG 10     // Cuántas conexiones pendientes se mantienen en cola

    void sigchld_handler(int s)
    {
        while(wait(NULL) > 0);
    }

    void control_error_conexion(int bytes_leidos, struct sockaddr_in cliente, int fd_cliente) {

    	if(bytes_leidos == 0) {
    		printf("Se desconecto el cliente %s\n de %i\n", inet_ntoa(cliente.sin_addr), fd_cliente);
    		exit(1);
    	}
    	if(bytes_leidos < 0) {
    		perror("recv");
    		exit(-1);
    	}

    }

    int main(void)
    {
        int sockfd, new_fd;  // Escuchar sobre sock_fd, nuevas conexiones sobre new_fd
        struct sockaddr_in my_addr;    // información sobre mi dirección
        struct sockaddr_in their_addr; // información sobre la dirección del cliente
        int sin_size;
        struct sigaction sa;
        int yes=1;

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        
        my_addr.sin_family = AF_INET;         // Ordenación de bytes de la máquina
        my_addr.sin_port = htons(MYPORT);     // short, Ordenación de bytes de la red
        my_addr.sin_addr.s_addr = inet_addr("192.168.43.135");//INADDR_ANY; // Rellenar con mi dirección IP
        memset(&(my_addr.sin_zero), '\0', 8); // Poner a cero el resto de la estructura

        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))
                                                                       == -1) {
            perror("bind");
            exit(1);
        }

        if (listen(sockfd, BACKLOG) == -1) {
            perror("listen");
            exit(1);
        }

        sa.sa_handler = sigchld_handler; // Eliminar procesos muertos
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            exit(1);
        }

		printf("Estoy escuchando, invita a tus amigos al server %s on "
                                "socket %d\n", inet_ntoa(my_addr.sin_addr), MYPORT);
			


        while(1) {  // main accept() loop
            sin_size = sizeof(struct sockaddr_in);
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                                                           &sin_size)) == -1) {
                perror("accept");
                continue;
            }
            printf("\nserver: got connection from %s in %i\n", inet_ntoa(their_addr.sin_addr), new_fd);
            if (!fork()) { // Este es el proceso hijo

                close(sockfd); // El hijo no necesita este descriptor
                char* mensajeCliente;
            	int tamanio;
            	int nbytesTAM;
            	int nbytesMSJ;
            	char tamanioMSJ[1];

            	char miSaludo[] = "\nTe conectaste con marichann. Bienvenido\n ~~~~ * ~~~~\n";
                if (send(new_fd, miSaludo, strlen(miSaludo), 0) == -1)
                    perror("send");

                while(1) {

                	nbytesTAM = recv(new_fd, tamanioMSJ,1, 0);
                	control_error_conexion(nbytesTAM, their_addr, new_fd);
                	tamanio = tamanioMSJ[0];
                	mensajeCliente = malloc(tamanio + 1);
                	nbytesMSJ = recv(new_fd, mensajeCliente,tamanio, 0);
                	control_error_conexion(nbytesMSJ, their_addr, new_fd);
                	mensajeCliente[tamanio] = '\0';
                	printf("\n%s dice >>> %s \n", inet_ntoa(their_addr.sin_addr), mensajeCliente);
                }

                close(new_fd);
                exit(0);
            }
            close(new_fd);  // El proceso padre no lo necesita
        }

        return 0;

    } 
