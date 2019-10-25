#include <stdio.h>
#include <stdlib.h>

#include <stddef.h>
#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "hablar-servidor.h"
#include "estructuras-fs.h"


/*Funciones de FUSE - Callbacks
 * Crear, escribir y borrar archivos
 * Crear y listar directorios y sus archivos
 * Eliminar directorios
 * Describir directorios y archivos
 * */

void *sac_init(struct fuse_conn_info *conn) {return 0;}

int sac_mknod(const char *path, mode_t mode, dev_t dev) {
	return 0;
}

/** Set access and modification time, with nanosecond resolution.
 * The arguments are the number of nanoseconds since jan 1 1970 00:00.**/
int sac_utimens (const char *path, const struct timespec tv[2]) {return 0;}

int sac_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi) {return 0;}

int sac_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("\n ===> bb_read(path=\"%s\", size=%d, offset=%lld)\n",
	    path,  size, offset);
	buf = pedir_oper_sacServer(path, 5);

	return strlen(buf);//número de bytes leidos
}

int sac_unlink(const char *path) {return 0;}
/* Para descrbir archivos y directorios */
int sac_getattr(const char *path, struct stat *statbuf) {return 0;}

int sac_mkdir(const char *path, mode_t mode) {return 0;}

int sac_rmdir(const char *path) {return 0;}


struct fuse_operations sac_cli_operations = {
		.init = sac_init,
		.mknod = sac_mknod,
		.utimens = sac_utimens,
		.write = sac_write,
		.read = sac_read,
		.unlink = sac_unlink,
		.getattr = sac_getattr,
		.mkdir = sac_mkdir,
		.rmdir = sac_rmdir,
};


int main(int argc, char* argv[]) {



	int fuse_stat;
	/**
	 * Create a new FUSE filesystem.
	 *
	 * @param ch the communication channel
	 * @param args argument vector
	 * @param op the filesystem operations
	 * @param op_size the size of the fuse_operations structure
	 * @param user_data user data supplied in the context during the init() method
	 * @return the created FUSE handle
	 */

	printf("fuse stat %i\n", fuse_stat);
	fuse_stat = fuse_main(argc, argv, &sac_cli_operations, NULL);
	return fuse_stat;
}
