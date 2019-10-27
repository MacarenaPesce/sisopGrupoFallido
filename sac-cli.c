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

int sac_read(const char *path, char *buffer, size_t size, off_t offset, struct fuse_file_info *fi) {
    printf("\n ~~~ sac_read  (path = \"%s\", size = %d, offset = %lld) ~~~\n",
	    path,  size, offset);

    printf("\n **** voy a pedir la operación... ****\n");
	size = sac_leer(path, buffer);
	return size; //número de bytes leidos
}

int sac_unlink(const char *path) {return 0;}
/* Para descrbir archivos y directorios */
int sac_getattr(const char *path, struct stat *statbuf) {

	printf("\n~~~getattr fue llamado con el path %s ~~~\n", path);// USAR STAT?
	statbuf->st_uid = getuid();  //Usurio y grupo que montaron el file system
	statbuf->st_gid = getgid();  //Preguntar si es correcto
	statbuf->st_atime = time( NULL );
	statbuf->st_mtime = time( NULL );

	if ( strcmp( path, "/" ) == 0 )
	{
		statbuf->st_mode = S_IFDIR | 0755; 	// r = 4 w = 2 x = 1
		statbuf->st_nlink = 2;  			//" hardlinks: . y ..
	}
	else  //acá puedo poner if(gfile.estado ...)
	{
		statbuf->st_mode = S_IFREG | 0644;
		statbuf->st_nlink = 1;
		//statbuf->st_size = 1024; Predirle el tamaño a SAC-SERVER
	} //Agregar control de error


	return 0;
}

int sac_mkdir(const char *path, mode_t mode) {return 0;}

int sac_rmdir(const char *path) {return 0;}

int sac_opendir (const char *path, struct fuse_file_info *fi) {
	return 0;
}

int sac_readdir( const char *path, void *buffer, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi ) {
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") != 0) //Temporal. Solo para probar
		return -ENOENT;
	printf("\n~~~ Leyendo el direcroirio %s ~~~\n", path);
	filler( buffer, ".", NULL, 0 );
	filler( buffer, "..", NULL, 0);
	filler(buffer, "jaja", NULL, 0);

	return 0;
}

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
		.opendir = sac_opendir,
		.readdir = sac_readdir,
};


int main(int argc, char* argv[]) {

	int fuse_stat;
	datos_server();


    if ((getuid() == 0) || (geteuid() == 0)) {
    	printf("Ejecutar sac-cli como root ocasiona agujeros de seguridad inaceptables\n");
    	return 1;
    }

	fuse_stat = fuse_main(argc, argv, &sac_cli_operations, NULL);
	printf("\n~~~fuse_main retornó %i ~~~\n", fuse_stat);

	return fuse_stat;
}
