/* log.h
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

/**
 * @brief	Add brief description!
 *
 * More detailed description
 *
 * @date	05/29/2018
 * @file	log.h
 * @author	Konrad Lother
 */

#ifndef LOG_H
#define LOG_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

#define LOG_FORMAT_DEFAULT "%D %T [%N] %L %f:%m:%l: %M"
#define LOG_FORMAT_DATE "%Y-%m-%d"
#define LOG_FORMAT_TIME "%H:%M:%S"

#define LOG(logger, level, ...) log_print(logger, level, __FILE__, __func__, __LINE__, __VA_ARGS__)
#define LOG_INFO(logger, ...) LOG(logger, LINFO, __VA_ARGS__)
#define LOG_WARNING(logger, ...) LOG(logger, LWARNING, __VA_ARGS__)
#define LOG_ERROR(logger, ...) LOG(logger, LERROR, __VA_ARGS__)
#define LOG_FATAL(logger, ...) LOG(logger, LFATAL, __VA_ARGS__)
#define LOG_DEBUG(logger, ...) LOG(logger, LDEBUG, __VA_ARGS__)
#define LOG_CRITICAL(logger, ...) LOG(logger, LCRITICAL, __VA_ARGS__)

enum log_level {
        LOFF = 0,
        LFATAL = 1,
        LCRITICAL = 2,
        LERROR = 4,
        LWARNING = 8,
        LINFO = 16,
        LDEBUG = 32,
        LALL = 64
};

struct log_appender {
        struct log_appender *next;
        struct log_appender *prev;
        void (*append)(const char *msg);
};

struct logger {
        char *progname;
        int level;
        char *format;
        char *format_date;
        char *format_time;
        struct log_appender *appenders;
        unsigned char initialized;
};


int log_init(struct logger *logger);
int log_free(struct logger *logger);
int log_appender(struct logger *logger, void (*appender)(const char *msg));
void log_print(struct logger *logger, enum log_level level, const char *path, const char *func, unsigned int line, char *format, ...);
// START HERE

#ifdef __cplusplus
}
#endif
#endif
