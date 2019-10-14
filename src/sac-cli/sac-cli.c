#include <stdio.h>
#include <stdlib.h>

#include <stddef.h>
#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <commons/config.h>

/*Funciones de FUSE - Callbacks
 * Crear, escribir y borrar archivos
 * Crear y listar directorios y sus archivos
 * Eliminar directorios
 * Describir directorios y archivos
 * */

void *sac_init(struct fuse_conn_info *conn) {}

int sac_mknod(const char *path, mode_t mode, dev_t dev) {}

/** Set access and modification time, with nanosecond resolution.
 * The arguments are the number of nanoseconds since jan 1 1970 00:00.**/
int sac_utimens(char* path, int atime, int mtime) {}

int sac_write(const char *path, const char *buf, size_t size, off_t offset,
	     struct fuse_file_info *fi) {}

int sac_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {}

int sac_unlink(const char *path) {}
/* Para descrbir archivos y directorios */
int sac_getattr(const char *path, struct stat *statbuf) {}

int sac_mkdir(const char *path, mode_t mode) {}

int sac_rmdir(const char *path) {}


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

	char* ip_server = malloc(15); //Suficiente para una dirección IP. Ver después
	int puerto_server;
	t_config* config = config_create("config-cli.config");
	ip_server = config_get_string_value("IP_SERVER");
	puerto_server = config_get_int_value("PUERTO_SERVER");

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

	fuse_stat = fuse_main(argc, argv, &sac_cli_operations, NULL);
	return fuse_stat;
}
