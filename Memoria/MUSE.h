/*
 *		24/10/2019	
 * 		Archivo Header de MUSE 
*/

#ifndef MUSE_H_
#define MUSE_H_

#include "libmuse.h"
#include <stdio.h>
#include <stdlib.h>
#include <commons/log.h>
#include <commons/config.h>
#include <commons/string.h>

/* Estructura del .config */

typedef struct config{
	int LISTEN_PORT;
	int MEMORY_SIZE;
	int PAGE_SIZE;
	int SWAP_SIZE;
    //Agregado por nosotros:
    char * RUTA_LOG; //se usa para iniciar_log: es el nombre del archivo .log
    t_log_level LOG_LEVEL; //se usa para iniciar_log: es el nivel de detalle minimo a loguear
};

/* Estructura de Metadata */

typedef struct tHeapMeta{
	uint32_t size;
	bool isFree;
};

t_log *g_logger;
t_config *g_config;
config configuracion;
void iniciarConfiguracion();
void iniciarLog();
char *obtenerPorClave(char *clave);

#endif /* MUSE_H_ */