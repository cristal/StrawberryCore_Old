/*
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2011 Strawberry Project <http://www.strawberry-pr0jcts.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef md5_INCLUDED
#  define md5_INCLUDED

/*
 * This package supports both compile-time and run-time determination of CPU
 * byte order.  If ARCH_IS_BIG_ENDIAN is defined as 0, the code will be
 * compiled to run only on little-endian CPUs; if ARCH_IS_BIG_ENDIAN is
 * defined as non-zero, the code will be compiled to run only on big-endian
 * CPUs; if ARCH_IS_BIG_ENDIAN is not defined, the code will be compiled to
 * run on either big- or little-endian CPUs, but will run slightly less
 * efficiently on either one than if ARCH_IS_BIG_ENDIAN is defined.
 */

typedef unsigned char md5_byte_t;                           /* 8-bit byte */
typedef unsigned int md5_word_t;                            /* 32-bit word */

/* Define the state of the MD5 Algorithm. */
typedef struct md5_state_s
{
    md5_word_t count[2];                                    /* message length in bits, lsw first */
    md5_word_t abcd[4];                                     /* digest buffer */
    md5_byte_t buf[64];                                     /* accumulate block */
} md5_state_t;

#ifdef __cplusplus
extern "C"
{
    #endif

    /* Initialize the algorithm. */
    void md5_init(md5_state_t *pms);

    /* Append a string to the message. */
    void md5_append(md5_state_t *pms, const md5_byte_t *data, int nbytes);

    /* Finish the message and return the digest. */
    void md5_finish(md5_state_t *pms, md5_byte_t digest[16]);

    #ifdef __cplusplus
}                                                           /* end extern "C" */
#endif
#endif                                                      /* md5_INCLUDED */

