#include"client.h"

#define IP "127.0.0.1"
#define PUERTO "1024"

int connectToServer(char *ip, char *PORT) {

	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;       // Permite que la maquina se encargue de verificar si usamos IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;   // Indica que usaremos el protocolo TCP
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, PORT, &hints, &server_info);     // Carga en serverInfo los datos de la conexion

	//Obtiene un socket (un file descriptor -todo en linux es un archivo-), utilizando la estructura serverInfo que generamos antes.
	int client_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(client_socket, server_info->ai_addr, server_info->ai_addrlen) == -1){
//		printf("Error al conectarse al servidor. Try again later.\n");
		freeaddrinfo(server_info); //ADROINFO
		close(client_socket);
		return -1;
	}
	freeaddrinfo(server_info); // No lo necesitamos mas

	return client_socket;
}

void sendMessage(int serverSocket,char *message){
	send(serverSocket, message, strlen(message) + 1, 0);
}

void sendMessages(int serverSocket) {
	int enviar = 1;
	char message[PACKAGESIZE];

	printf("Conectado al servidor. Bienvenido al sistema, ya puede enviar mensajes. Escriba 'exit' para salir\n");

	while(enviar){
		printf(">");
		fgets(message, PACKAGESIZE, stdin); // Lee una linea en el stdin (lo que escribimos en la consola) hasta encontrar un \n (y lo incluye) o llegar a PACKAGESIZE.
		sprintf (message, "%c%s", strlen(message), message);
		if(strcasecmp("exit\n", message) == 0) enviar = 0;  // Chequeo que el usuario no quiera salir
		if (enviar) send(serverSocket, message, strlen(message) + 1, 0);  // Solo envio si el usuario no quiere salir.
		
	}
}

void closeConnection(int serverSocket) {
	close(serverSocket);
}






int main(){
	
	//Me conecto	
	int client_socket= connectToServer(IP,PUERTO);

	//creamos el paquete y enviamos mensajes
	sendMessages(client_socket);

	// cierro conexion 
	closeConnection(client_socket);

	return 0;

	
}

