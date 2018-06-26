
/* rstrcat.c: ADD DESCRIPTION HERE
 *
 * Copyright (C) 2018
 *	Konrad Lother <k@hiddenbox.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "rstrcat.h"

int rstrcat(char **dst, char *src)
{
    size_t src_s, dst_s, new_s;
    char *p;

    if (src == NULL)
            return -1;

    if (*dst == NULL) {
        *dst = calloc(1, sizeof(char));
        if (*dst == NULL)
            return -1;
    }
    src_s = strlen(src);
    dst_s = strlen(*dst);
    new_s = src_s + dst_s;

    p = calloc(new_s + 1, sizeof(char));
    if (p == NULL)
        return -1;

    memcpy(p, *dst, dst_s);
    memcpy(p+dst_s, src, src_s);
    *dst = p;

    return new_s;
}

int frstrcat(char **dst, char *fmt, ...)
{
    int size = 0;
    va_list args;

    va_start(args, fmt);
    size = vfrstrcat(dst, fmt, args);
    va_end(args);

    return size;
};

int vfrstrcat(char **dst, char *fmt, va_list args)
{
    int size = 0;
    char *str;
    va_list tmp;

    va_copy(tmp, args);
    size = vsnprintf(NULL, size, fmt, tmp);
    va_end(tmp);

    if (size < 0)
        return -1;

    str = calloc(size+1, sizeof(char));
    size = vsnprintf(str, size+1, fmt, args);
    if (size < 0) {
        free(str);
        return -1;
    }

    size = rstrcat(dst, str);
    if (size < 0) {
        free(str);
        return -1;
    }
    free(str);

    return size;
}
