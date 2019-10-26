/*
 *		2/10/2019	
 * 		Módulo de MUSE 
*/

#include "muse.h"

int main(int argc, char *argv[]){

	iniciarConfiguracion();
	iniciarLog();

	/* COSAS DE EJEMPLO DE LA CÁTEDRA */

	uint32_t my_memory = muse_alloc(10); //única porción central de memoria
	int memory_x = 10;
	int*memory_y = malloc(4);
	muse_cpy(my_memory, &memory_x, 4); //copies 4 bytes from local memory_x area to my_memory area
	muse_get(memory_y, my_memory, 4); //copies 4 bytes from my_memory area to local memory_y area.
	printf("%d", *memory_y);

	//Memoria Compartida
	size_t filesize = 16;

	int MAP_PRIVATE = 1;	//esto pa' que compile
	uint32_t map = muse_map("hola.txt", filesize, MAP_PRIVATE); 
	   /*	
	   		opens the "hola.txt" file in RDONLY mode
	   		map is a position _mapped_ of pages of a given 'filesize' of the "hola.txt" file  
			muse_map basically is just putting the hola.txt file in memory    
   		*/	

	printf(map, filesize);
	int x = 4;
	muse_cpy(map, &x, 4); //copies 4 bytes from local memory_x area to my_memory area
	muse_sync(map, filesize); //drop a 'filesize' amount of bytes and write them to the file in FileSystem

	muse_unmap(map); 

	/* FIN DE COSAS DE LA CÁTEDRA */
	
/* PARA HACER=

	//int UPCM = malloc(1000) //única porción central de memoria

	void* asignameRAM(){
		
		buscoSegmentosQNoEstenLLenos()
		
		if(puedo llenar el segmento que todavía no está lleno)
			llenarlo
		else if(puedo agrandar ese segmento) 
			agrandarlo
		else if(puedo pedir un nuevo segmento)
			pedirlo 
		else
			tengo llena la memoria 
		log()
	}
	monitoriarProceso()	

*/
	terminarMuse();
	
}	

void iniciarConfiguracion(){
	puts("Configuracion:");
	rutaConfig = "/home/utnso/git/tp-2019-2c-nochesDePasionDeC/Memoria/muse.config";
	config = config_create(rutaConfig);

	configuracion.LISTEN_PORT = atoi(obtenerPorClave("LISTEN_PORT"));
	configuracion.MEMORY_SIZE = atoi(obtenerPorClave("MEMORY_SIZE"));
	configuracion.PAGE_SIZE = atoi(obtenerPorClave("PAGE_SIZE"));
	configuracion.SWAP_SIZE = atoi(obtenerPorClave("SWAP_SIZE"));
	//agregados x nosotros
	configuracion.RUTA_LOG = obtenerPorClave("RUTA_LOG");
	configuracion.LOG_LEVEL =  log_level_from_string(obtenerPorClave("LOG_LEVEL"));
}

void iniciarLog(){
	logger = log_create(configuracion.RUTA_LOG, "Memoria", 1, configuracion.LOG_LEVEL);
	loggear_debug(string_from_format("Logger en modo %s \n",log_level_as_string(configuracion.LOG_LEVEL)));
}

char *obtenerPorClave(char *clave){
	char *valor;
	valor = config_get_string_value(config, clave);
	printf(" %s: %s \n", clave, valor);
	puts("Valor obtenido");
	return valor;
}

void terminarMuse(){

	log_destroy(logger);
	config_destroy(config);

	puts("Gracias por usar el gestor de memoria central!");
	exit(0);
}

void monitoriarProceso(){
	/*int memoriaTPedida;
	int memoriaTLiberada;
	int memoryLeaksT;

	if(termino) log()*/
}