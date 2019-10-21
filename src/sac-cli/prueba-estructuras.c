#include "estructuras-fs.h"
#include "hablar-servidor.h"
#include <commons/config.h>

int main() {

	datos_server();
	printf("puerto servidor: %i\n ip: %s\n", puerto_server, ip_server);

	pedir_oper_sacServer("quiero que leas", 5); // 5 significa leer
	//pedir_oper_sacServer("amamee", 8);
	//printf("Tamaño de Gfile: %i \n", sizeof(Gfile));
	//printf("Tamaño de Gbloque: %i \n", sizeof(Gbloque));
	return 0;
}

