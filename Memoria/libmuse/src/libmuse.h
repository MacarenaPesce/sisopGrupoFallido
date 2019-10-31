/*
 * Copyright (C) 2019 Sistemas Operativos - UTN FRBA. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBMUSE_H_
#define LIBMUSE_H_

    #include <stdint.h>
    #include <stddef.h>

    /**
     * Inicializa la biblioteca de MUSE.
     * @param id El Process (o Thread) ID para identificar el caller en MUSE.
     * @param ip IP en dot-notation de MUSE.
     * @param puerto Puerto de conexi�n a MUSE.
     * @return Si pasa un error, retorna -1. Si se inicializ� correctamente, retorna 0.
     * @see Para obtener el id de un proceso pueden usar getpid() de la lib POSIX (unistd.h)
     * @note Debido a la naturaleza centralizada de MUSE, esta funci�n deber� definir
     *  el ID del proceso/hilo seg�n "IP-ID".
     */
    int muse_init(int id, char* ip, int puerto);

    /**
     * Cierra la biblioteca de MUSE.
     */
    void muse_close();

    /**
     * Reserva una porci�n de memoria cont�gua de tama�o `tam`.
     * @param tam La cantidad de bytes a reservar.
     * @return La direcci�n de la memoria reservada.
     */
    uint32_t muse_alloc(uint32_t tam);

    /**
     * Libera una porci�n de memoria reservada.
     * @param dir La direcci�n de la memoria a reservar.
     */
    void muse_free(uint32_t dir);

    /**
     * Copia una cantidad `n` de bytes desde una posici�n de memoria de MUSE a una `dst` local.
     * @param dst Posici�n de memoria local con tama�o suficiente para almacenar `n` bytes.
     * @param src Posici�n de memoria de MUSE de donde leer los `n` bytes.
     * @param n Cantidad de bytes a copiar.
     * @return Si pasa un error, retorna -1. Si la operaci�n se realiz� correctamente, retorna 0.
     */
    int muse_get(void* dst, uint32_t src, size_t n);

    /**
     * Copia una cantidad `n` de bytes desde una posici�n de memoria local a una `dst` en MUSE.
     * @param dst Posici�n de memoria de MUSE con tama�o suficiente para almacenar `n` bytes.
     * @param src Posici�n de memoria local de donde leer los `n` bytes.
     * @param n Cantidad de bytes a copiar.
     * @return Si pasa un error, retorna -1. Si la operaci�n se realiz� correctamente, retorna 0.
     */
    int muse_cpy(uint32_t dst, void* src, int n);


    /**
     * Devuelve un puntero a una posici�n mappeada de p�ginas por una cantidad `length` de bytes el archivo del `path` dado.
     * @param path Path a un archivo en el FileSystem de MUSE a mappear.
     * @param length Cantidad de bytes de memoria a usar para mappear el archivo.
     * @param flags
     *          MAP_PRIVATE     Solo un proceso/hilo puede mappear el archivo.
     *          MAP_SHARED      El segmento asociado al archivo es compartido.
     * @return Retorna la posici�n de memoria de MUSE mappeada.
     * @note: Si `length` sobrepasa el tama�o del archivo, toda extensi�n deber� estar llena de "\0".
     * @note: muse_free no libera la memoria mappeada. @see muse_unmap
     */
    uint32_t muse_map(char *path, size_t length, int flags);

    /**
     * Descarga una cantidad `len` de bytes y lo escribe en el archivo en el FileSystem.
     * @param addr Direcci�n a memoria mappeada.
     * @param len Cantidad de bytes a escribir.
     * @return Si pasa un error, retorna -1. Si la operaci�n se realiz� correctamente, retorna 0.
     * @note Si `len` es menor que el tama�o de la p�gina en la que se encuentre, se deber� escribir la p�gina completa.
     */
    int muse_sync(uint32_t addr, size_t len);

    /**
     * Borra el mappeo a un archivo hecho por muse_map.
     * @param dir Direcci�n a memoria mappeada.
     * @param
     * @note Esto implicar� que todas las futuras utilizaciones de direcciones basadas en `dir` ser�n accesos inv�lidos.
     * @note Solo se deber� cerrar el archivo mappeado una vez que todos los hilos hayan liberado la misma cantidad de muse_unmap que muse_map.
     * @return Si pasa un error, retorna -1. Si la operaci�n se realiz� correctamente, retorna 0.
     */
    int muse_unmap(uint32_t dir);
#endif
