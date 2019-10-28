#include libmuse.h

int muse_init(int id, char* ip, int puerto){
    /**
     * Inicializa la biblioteca de MUSE.
     * @param id El Process (o Thread) ID para identificar el caller en MUSE.
     * @param ip IP en dot-notation de MUSE.
     * @param puerto Puerto de conexión a MUSE.
     * @return Si pasa un error, retorna -1. Si se inicializó correctamente, retorna 0.
     * @see Para obtener el id de un proceso pueden usar getpid() de la lib POSIX (unistd.h)
     * @note Debido a la naturaleza centralizada de MUSE, esta función deberá definir
     *  el ID del proceso/hilo según "IP-ID".
     */
	//tengo que tener en cuenta los errores, si no es posible la operacion etc
	//necesito saber como inicializar muse
	//validar los datos
	//ver que mierda me falta ademas de eso.....
}

void muse_close(){
	//Libero espacio que use muse y lo vacio 
	//close usar
	//funciones para cerrar
}

uint32_t muse_alloc(uint32_t tam){
    /**
     * Reserva una porción de memoria contígua de tamaño `tam`.
     * @param tam La cantidad de bytes a reservar.
     * @return La dirección de la memoria reservada.
     */
	//reservar posicion, buscar cual es la funcion que usa memoria, algo asi con malloc que reserva memoria
	//validar que haya la cantidad de memoria que pide reservar
	//    return (uint32_t) malloc(tam);
}    

void muse_free(uint32_t dir){
    //free((void*) dir);
    
     /**
     * Libera una porción de memoria reservada.
     * @param dir La dirección de la memoria a reservar.
     */
	//liberar memoria : free , otra cosa no se me ocurre
}

int muse_get(void* dst, uint32_t src, size_t n){
    //memcpy(dst, (void*) src, n);
    
     /**
     * Copia una cantidad `n` de bytes desde una posición de memoria de MUSE a una `dst` local.
     * @param dst Posición de memoria local con tamaño suficiente para almacenar `n` bytes.
     * @param src Posición de memoria de MUSE de donde leer los `n` bytes.
     * @param n Cantidad de bytes a copiar.
     * @return Si pasa un error, retorna -1. Si la operación se realizó correctamente, retorna 0.
     */
	//tengo 2 casos o 2 if, o un if y un retorno fuera.
	//chequear que la operacion sea correcta
	//comando cpy o algo asi para copiar
	//validar que exista la posicion src, que se puedan leer los n bytes
}

int muse_cpy(uint32_t dst, void* src, int n){
    //memcpy((void*) dst, src, n);
    
     /**
     * Copia una cantidad `n` de bytes desde una posición de memoria local a una `dst` en MUSE.
     * @param dst Posición de memoria de MUSE con tamaño suficiente para almacenar `n` bytes.
     * @param src Posición de memoria local de donde leer los `n` bytes.
     * @param n Cantidad de bytes a copiar.
     * @return Si pasa un error, retorna -1. Si la operación se realizó correctamente, retorna 0.
     */
	//parecido al anterior, cambia que es en MUSE
	//entender cual es la diferencia entre local y muse para copiar
}

uint32_t muse_map(char *path, size_t length, int flags){
    /**
     * Devuelve un puntero a una posición mappeada de páginas por una cantidad `length` de bytes el archivo del `path` dado.
     * @param path Path a un archivo en el FileSystem de MUSE a mappear.
     * @param length Cantidad de bytes de memoria a usar para mappear el archivo.
     * @param flags
     *          MAP_PRIVATE     Solo un proceso/hilo puede mappear el archivo.
     *          MAP_SHARED      El segmento asociado al archivo es compartido.
     * @return Retorna la posición de memoria de MUSE mappeada.
     * @note: Si `length` sobrepasa el tamaño del archivo, toda extensión deberá estar llena de "\0".
     * @note: muse_free no libera la memoria mappeada. @see muse_unmap
     */
	//ver como se mapea una posicion de memoria
	//Ver como se relaciona un Path con el FS
	//validar la cantidad de bytes (length) y tener en cuenta la nota
	// ver los flags y las diferencias
	// ¿de donde salen los flags?
}

int muse_sync(uint32_t addr, size_t len){
    /**
     * Descarga una cantidad `len` de bytes y lo escribe en el archivo en el FileSystem.
     * @param addr Dirección a memoria mappeada.
     * @param len Cantidad de bytes a escribir.
     * @return Si pasa un error, retorna -1. Si la operación se realizó correctamente, retorna 0.
     * @note Si `len` es menor que el tamaño de la página en la que se encuentre, se deberá escribir la página completa.
     */
	//RELACION CON FS , Hablar con Mari esa relacion
	//validar la cantidad de bytes, tener en cuenta la nota
	//doble retorno, un if
}

int muse_unmap(uint32_t dir){
    /**
     * Borra el mappeo a un archivo hecho por muse_map.
     * @param dir Dirección a memoria mappeada.
     * @note Esto implicará que todas las futuras utilizaciones de direcciones basadas en `dir` serán accesos inválidos.
     * @note Solo se deberá cerrar el archivo mappeado una vez que todos los hilos hayan liberado la misma cantidad de muse_unmap que muse_map.
     * @return Si pasa un error, retorna -1. Si la operación se realizó correctamente, retorna 0.
     */
	//tengo 2 opciones para retornar, hay un if
	//ver el tema de mappeo, entenderlo
	//entender las notas ¿?
	
}


/**
	* @DESC: Crea y devuelve un puntero a una estructura t_bitarray con formato LSB_FIRST
	*		bitarray - el bloque de memoria que contiene los bits a leer / escribir
	*		size - la cantidad de bits del bitarray, expresada en bytes (1 byte = 8 bits)
	*		ejemplo: bitarray de 8 posiciones (bits), 
	*			void* puntero_a_bits = //un byte de memoria, como por ejemplo malloc(1)
	*			bitarray_create(puntero_a_bits, 1)
	*/
	t_bitarray 	*bitarray_create(char *bitarray, size_t size);

______________________

* @DESC: Crea y devuelve un puntero a una estructura t_bitarray
	* @PARAMS:
	* 		bitarray - el bloque de memoria que contiene los bits a leer/escribir
	*		size - la cantidad de bits del bitarray, expresada en bytes (1 byte = 8 bits)
	*		mode - LSB_FIRST ó MSB_FIRST
	*
	*		ejemplo: bitarray de 8 posiciones (bits) con LSB_FIRST, 
	*			void* puntero_a_bits = //un byte de memoria, como por ejemplo malloc(1)
	*			bitarray_create_with_mode(puntero_a_bits, 1, LSB_FIRST)
	*/
	t_bitarray	*bitarray_create_with_mode(char *bitarray, size_t size, bit_numbering_t mode);

_______________________

* @DESC: Devuelve el valor del bit de la posicion indicada
	*/
	bool 		 bitarray_test_bit(t_bitarray*, off_t bit_index);
__________________________________
/**
	* @NAME: bitarray_set_bit
	* @DESC: Setea el valor del bit de la posicion indicada
	*/
	void		 bitarray_set_bit(t_bitarray*, off_t bit_index);

___________________________________

	/**
	* @NAME: bitarray_clean_bit
	* @DESC: Limpia el valor del bit de la posicion indicada
	*/
	void		 bitarray_clean_bit(t_bitarray*, off_t bit_index);

___________________________________

	/**
	* @NAME: bitarray_get_max_bit
	* @DESC: Devuelve la cantidad de bits en el bitarray
	*/
	size_t		 bitarray_get_max_bit(t_bitarray*);

____________________________________
	/**
	* @NAME: bitarray_destroy
	* @DESC: Destruye el bit array
	*/
	void 		 bitarray_destroy(t_bitarray*);
____________________________________

	/**
	* @NAME: error_show
	* @DESC: imprime un mensaje con el siguiente formato
	*
	* 	[[ERROR]] MESSAGE
	*/
	void error_show(char *message, ...);