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

____________________________________________

LEER BIEEEEEEN

/**
	* @NAME: log_create
	* @DESC: Crea una instancia de logger, tomando por parametro
	* el nombre del programa, el nombre del archivo donde se van a generar los logs,
	* el nivel de detalle minimo a loguear y si además se muestra por pantalla lo que se loguea.
	*/
	t_log* 		log_create(char* file, char *program_name, bool is_active_console, t_log_level level);

	/**
	* @NAME: log_destroy
	* @DESC: Destruye una instancia de logger
	*/
	void 		log_destroy(t_log* logger);

	/**
	* @NAME: log_trace
	* @DESC: Loguea un mensaje con el siguiente formato
	*
	* [TRACE] hh:mm:ss:mmmm PROCESS_NAME/(PID:TID): MESSAGE
	*
	*/
	void 		log_trace(t_log* logger, const char* message, ...);

	/**
	* @NAME: log_debug
	* @DESC: Loguea un mensaje con el siguiente formato
	*
	* [DEBUG] hh:mm:ss:mmmm PROCESS_NAME/(PID:TID): MESSAGE
	*
	*/
	void 		log_debug(t_log* logger, const char* message, ...);

	/**
	* @NAME: log_info
	* @DESC: Loguea un mensaje con el siguiente formato
	*
	* [INFO] hh:mm:ss:mmmm PROCESS_NAME/(PID:TID): MESSAGE
	*
	*/
	void 		log_info(t_log* logger, const char* message, ...);

	/**
	* @NAME: log_warning
	* @DESC: Loguea un mensaje con el siguiente formato
	*
	* [WARNING] hh:mm:ss:mmmm PROCESS_NAME/(PID:TID): MESSAGE
	*
	*/
	void 		log_warning(t_log* logger, const char* message, ...);

	/**
	* @NAME: log_error
	* @DESC: Loguea un mensaje con el siguiente formato
	*
	* [ERROR] hh:mm:ss:mmmm PROCESS_NAME/(PID:TID): MESSAGE
	*
	*/
	void 		log_error(t_log* logger, const char* message, ...);

	/**
	* @NAME: log_level_as_string
	* @DESC: Convierte un t_log_level a su representacion en string
	*/
	char 		*log_level_as_string(t_log_level level);

	/**
	* @NAME: log_level_from_string
	* @DESC: Convierte un string a su representacion en t_log_level
	*/
	t_log_level log_level_from_string(char *level);


---------------------------------------------------------------------------------------

 /**
    * @NAME: mem_hexstring
    * @DESC: Devuelve un dump hexadecimal en formato string de una porción de memoria dada
    */
  char *mem_hexstring(void *source, size_t length);

  /**
    * @NAME: mem_hexdump
    * @DESC: Imprime un dump hexadecimal por pantalla de una porción de memoria dada
    */
  void mem_hexdump(void *source, size_t length);

----------------------------------------------------------------------------------------

/**
* @NAME: process_get_thread_id
* @DESC: Obtiene el ID del thread actual
*/
unsigned int process_get_thread_id();

/**
* @NAME: process_getpid
* @DESC: Obtiene el ID del proceso actual
*/
unsigned int process_getpid();

----------------------------------------------------------------------------------------

	/**
	* @NAME: string_new
	* @DESC: Crea un string vacio
	*/
	char*   string_new();

	/**
	* @NAME: string_itoa
	* @DESC: Crea un string a partir de un numero
	*/
	char* string_itoa(int number);

	/**
	* @NAME: string_from_format
	* @DESC: Crea un nuevo string a partir de un formato especificado
	*
	* Ejemplo:
	* char* saludo = string_from_format("Hola %s", "mundo");
	*
	* => saludo = "Hola mundo"
	*/
	char*   string_from_format(const char* format, ...);

	/**
	* @NAME: string_from_vformat
	* @DESC: Crea un nuevo string a partir de un formato especificado
	* pasando un va_list con los argumentos
	*/
	char*   string_from_vformat(const char* format, va_list arguments);

	/**
	* @NAME: string_repeat
	* @DESC: Crea un string de longitud 'count' con el mismo caracter.
	*
	* Ejemplo:
	* string_repeat('a', 5) = "aaaaa"
	*
	*/
	char*   string_repeat(char ch, int count);

	/**
	* @NAME: string_append
	* @DESC: Agrega al primer string el segundo
	*
	* Ejemplo:
	* char *unaPalabra = string_new();
	* string_append(&unaPalabra, "HOLA ");
	* string_append(&unaPalabra, "PEPE");
	*
	* => unaPalabra = "HOLA PEPE"
	*/
	void 	string_append(char ** original, char * string_to_add);

	/**
	* @NAME: string_append_with_format
	* @DESC: Concatena al primer string el resultado de aplicar los parametros al
	* formato especificado
	*
	* Ejemplo:
	* char *saludo = "HOLA ";
	* char *nombre = "PEPE";
	*
	* string_append_with_format(&saludo, "%s!", nombre);
	*
	* => saludo = "HOLA PEPE!"
	*/
	void    string_append_with_format(char **original, const char *format, ...);

	/**
	* @NAME: string_duplicate
	* @DESC: Retorna una copia del string pasado
	* como argumento
	*/
	char*	string_duplicate(char* original);

	/**
	* @NAME: string_to_upper
	* @DESC: Pone en mayuscula todos los caracteres de un string
	*/
	void 	string_to_upper(char * text);

	/**
	* @NAME: string_to_lower
	* @DESC: Pone en minuscula todos los caracteres de un string
	*/
	void 	string_to_lower(char * text);

	/**
	* @NAME: string_capitalized
	* @DESC: Capitaliza un string
	*/
	void 	string_capitalized(char * text);

	/**
	* @NAME: string_trim
	* @DESC: Remueve todos los caracteres
	* vacios de la derecha y la izquierda
	*/
	void 	string_trim(char ** text);

	/**
	* @NAME: string_trim_left
	* @DESC: Remueve todos los caracteres
	* vacios de la izquierda
	*/
	void 	string_trim_left(char ** text);

	/**
	* @NAME: string_trim_right
	* @DESC: Remueve todos los caracteres
	* vacios de la derecha
	*/
	void 	string_trim_right(char ** text);

	/**
	* @NAME: string_length
	* @DESC: Retorna la longitud del string
	*/
	int 	string_length(char * text);

	/**
	* @NAME: string_is_empty
	* @DESC: Retorna si un string es ""
	*/
	bool 	string_is_empty(char * text);

	/**
	* @NAME: string_starts_with
	* @DESC: Retorna un boolean que indica
	* si un string comienza con el
	* string pasado por parametro
	*/
	bool 	string_starts_with(char * text, char * begin);

	/**
	* @NAME: string_ends_with
	* @DESC: Retorna un boolean que indica
	* si un string finaliza con el
	* string pasado por parametro
	*/
	bool	string_ends_with(char* text, char* end);

	/**
	* @NAME: string_equals_ignore_case
	* @DESC: Retorna si dos strings son iguales
	* ignorando las mayusculas y minusculas
	*/
	bool 	string_equals_ignore_case(char * actual, char * expected);

	/**
	* @NAME: string_split
	* @DESC: Separa un string dado un separador
	*
	* @Return: Retorna un array con cada palabra y en la última posición un NULL
	*
	* Ejemplo:
	* string_split("hola, mundo", ",") => ["hola", " mundo", NULL]
	*/
	char**  string_split(char * text, char * separator);


	/**
	 * @NAME: string_n_split
	 * @DESC: Separa un string tantas veces por su separador como n lo permita
	 *
	 * Ejemplo:
	 * string_n_split("hola, mundo, bueno", 2, ",") => ["hola", " mundo, bueno", NULL]
	 * string_n_split("hola, mundo, bueno", 3, ",") => ["hola", " mundo", " bueno", NULL]
	 * string_n_split("hola, mundo, bueno", 10, ",") => ["hola", " mundo", " bueno", NULL]
	 *
	 */
	char**  string_n_split(char* text, int n, char* separator);

	/**
	* @NAME: string_substring
	* @DESC: Retorna los length caracteres de text empezando en start
	* en un nuevo string
	*/
	char*   string_substring(char* text, int start, int length);

	/**
	* @NAME: string_substring_from
	* @DESC: Retorna el substring de text desde el indice start hasta
	* el último de la palabra
	*/
	char*   string_substring_from(char *text, int start);

	/**
	* @NAME: string_substring_until
	* @DESC: Retorna los primeros length caracteres de text como un nuevo string
	*/
	char*   string_substring_until(char *text, int length);

	/**
	* @NAME: string_iterate_lines
	* @DESC: Itera un array de strings aplicando
	* el closure a cada string, hasta que encuentre un NULL
	*/
	void 	string_iterate_lines(char ** strings, void (*closure)(char *));

	/**
	* @NAME: string_get_string_as_array
	* @DESC: Retorna una array separando los elementos
	* de un string con formato de array
	*
	* Ejemplo:
	* char* array_string = "[1,2,3,4]"
	* string_get_value_as_array(array_string) => ["1","2","3","4"]
	*/
	char**  string_get_string_as_array(char* text);

	/**
	 * @NAME: string_reverse
	 * @DESC: Retorna el texto invertido. No se maneja el caso de NULL,
	 * si se pasa NULL su comportamiento no esta determinado.
	 *
	 * Ejemplo:
	 * char* original = "boo";
	 * string_reverse(original) => "oob"
	 */
	char*	string_reverse(char* text);

	/**
	 * @NAME: string_contains
	 * @DESC: Retorna un boolean que indica si text contiene o no
	 * a substring.
	 */
	bool	string_contains(char* text, char *substring);

--------------------------------------------------------------------------------------

	/**
	* @NAME: temporal_get_string_time
	* @DESC: Retorna un string con la hora actual,
	* con el siguiente formato: hh:mm:ss:mmmm
	*/
	char *temporal_get_string_time();

---------------------------------------------------------------------------------------

/**
* @NAME: txt_open_for_append
* @DESC: Abre un archivo para agregarle contenido al final
*/
FILE* txt_open_for_append(char* path);

/**
* @NAME: txt_write_in_file
* @DESC: Agrega contenido al final del archivo
*/
void txt_write_in_file(FILE* file, char* string);

/**
* @NAME: txt_write_in_stdout
* @DESC: Imprime un mensaje en la salida estandar
*/
void txt_write_in_stdout(char* string);

/**
* @NAME: txt_close_file
* @DESC: Cierra el archivo
*/
void txt_close_file(FILE* file);