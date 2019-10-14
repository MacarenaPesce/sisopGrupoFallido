#ifndef SAC_CLI_ESTRUCTURAS_FS_H_
#define SAC_CLI_ESTRUCTURAS_FS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 4096
#define MAX_FILE_SIZE 71
#define INDIR_SIMPLE 1000

typedef struct {
	char tam_bloque[BLOCK_SIZE];
} Gbloque;

typedef struct {
	uint8_t estado;
	char nombre_archivo[MAX_FILE_SIZE];
	Gbloque* bloque_padre; // debe ser de tipo ptrGbloque. Corregir
	uint32_t tam_archivo;
	double fecha_creacion;
	double fecha_modificacion;
	Gbloque* simple_indirect[INDIR_SIMPLE];
} Gfile;



#endif /* SAC_CLI_ESTRUCTURAS_FS_H_ */
