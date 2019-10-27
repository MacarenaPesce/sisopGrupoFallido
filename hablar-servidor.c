#include "hablar-servidor.h"

void datos_server() {
	config = config_create("config-cli.config");
	char* ip_valor = config_get_string_value(config, "IP_SERVER");

	strcpy(ip_server, ip_valor);
	puerto_server = config_get_int_value(config, "PUERTO_SERVER");
	config_destroy(config);
}


char* pedir_oper_sacServer(const char* pedido_oper, int codigo_oper) {

	int tamaniomensaje = 0;

	struct sockaddr_in direccionservidor;
	direccionservidor.sin_family = AF_INET;
	direccionservidor.sin_addr.s_addr = INADDR_ANY; //Cambiar por ip del servdor
	direccionservidor.sin_port = htons(puerto_server); //cambiar por puerto
	memset(&(direccionservidor.sin_zero), '\0', 8);

	int cliente = socket(AF_INET, SOCK_STREAM, 0);

	int servidor;
	if ((servidor = connect(cliente, (void*) &direccionservidor,
			sizeof(direccionservidor))) != 0) {
		perror("No me pude conectar.\n");
	}

	printf("Estoy aquí. : D\n");

	char *mensaje = malloc(strlen(pedido_oper) + 10);
	sprintf(mensaje, "%c%c%s", codigo_oper,strlen(pedido_oper), pedido_oper);//ej: 54hola (5 en char)
	send(cliente, mensaje, strlen(mensaje) + 1, 0);
	printf("lo que mando %s\n", mensaje);

	//Recibir la respuesta del servidor
	recv(cliente, mensaje, 1, 0);
	tamaniomensaje = mensaje[0]; //El tamaño está concatenado al mensaje que manda el servidor
	printf("la respuesta del servidor pesa %i\n", tamaniomensaje);
	recv(cliente, mensaje, tamaniomensaje, 0);
	mensaje[tamaniomensaje] = '\0';
	printf("%s\n", mensaje);

	close(cliente);
	close(servidor);
	return mensaje;
}

int sac_leer(const char* path, char* buffer) {
	strcpy(buffer, pedir_oper_sacServer(path, LEER_ARCHIVO));

	return strlen(buffer);
}

