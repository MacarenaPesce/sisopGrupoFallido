/*
 *		2/10/2019	
 * 		Módulo de MUSE 
*/

int main(int argc, char *argv[]){
	
	iniciarConfiguracion();
	iniciarLog();
	
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
		
	void monitoriarProceso(){
		int memoriaTPedida;
		int memoriaTLiberada;
		int memoryLeaksT;

		if(termino) log()
	}
*/
	terminarMuse()
	
}	

void iniciarConfiguracion(){
	puts("Configuracion:");
	rutaConfig = "/home/utnso/git/tp-2019-2c-nochesDePasionDeC/Memoria/muse.config";
	g_config = config_create(rutaConfig);

	configuracion.LISTEN_PORT = atoi(obtener_por_clave("LISTEN_PORT"));
	configuracion.MEMORY_SIZE = atoi(obtener_por_clave("MEMORY_SIZE"));
	configuracion.PAGE_SIZE = atoi(obtener_por_clave("PAGE_SIZE"));
	configuracion.SWAP_SIZE = atoi(obtener_por_clave("SWAP_SIZE"));
	//agregados x nosotros
	configuracion.RUTA_LOG = obtener_por_clave("RUTA_LOG");
	configuracion.LOG_LEVEL =  log_level_from_string(obtener_por_clave("LOG_LEVEL"));
}

void iniciarLog(){
	g_logger = log_create(configuracion.RUTA_LOG, "Memoria", 1, configuracion.LOG_LEVEL);
	loggear_debug(string_from_format("Logger en modo %s \n",log_level_as_string(configuracion.LOG_LEVEL)));
}

char *obtenerPorClave(char *clave){
	char *valor;
	valor = config_get_string_value(g_config, clave);
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
