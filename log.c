
/* log.c: ADD DESCRIPTION HERE
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <libgen.h>

#ifdef WITH_MATH
#include <math.h>
#endif

#include "log.h"

#define REALLOC_BUF(buf, size) \

static char *strloglevel(enum log_level level);

int log_init(struct logger *logger)
{
        if (logger == NULL)
                return (errno = EINVAL);

        logger->progname = NULL;
        logger->level = LOFF;
        logger->format = LOG_FORMAT_DEFAULT;
        logger->format_date = LOG_FORMAT_DATE;
        logger->format_time = LOG_FORMAT_TIME;
        logger->appenders = NULL;

        logger->initialized = 1;

        return 0;
}

int log_free(struct logger *logger)
{
        int i;
        struct log_appender *current;

        logger->progname = NULL;
        if (logger->appenders != NULL) {
                current = logger->appenders;
                while (current->next != NULL) {
                        current->next->prev = current;
                        current = current->next;
                }

                while (current->prev != NULL) {
                        current = current->prev;
                        free(current->next);
                        current->next = NULL;
                }

                free(current);
        }
        return 0;
}

int log_appender(struct logger *logger, void (*appender)(const char *))
{
        struct log_appender *current;

        if (logger->appenders == NULL) {
                logger->appenders = malloc(sizeof(struct log_appender));
                if (logger->appenders == NULL)
                        return (errno = ENOMEM);

                logger->appenders->next = NULL;
                logger->appenders->prev = NULL;
                logger->appenders->append = appender;
                return 0;
        }

        current = logger->appenders;

        while (current->next != NULL) {
                current->next->prev = current;
                current = current->next;
        }

        current->next = malloc(sizeof(struct log_appender));
        if (current->next == NULL)
                return (errno = ENOMEM);

        current->next->append = appender;
        return 0;
}

void log_print(struct logger *logger, enum log_level level, const char *path, const char *func, unsigned int line, char *format, ...)
{
        int r, msec;
        struct log_appender *current;
        char *buffer, *message, *tmp, *p, date_str[80], time_str[80];
        time_t rawtime;
        struct timeval tv;
        struct tm *tm;

        if (logger->appenders == NULL)
                return;

        if ((logger->level != LALL) && (0 == (logger->level & level)))
                return;

        message = NULL;
        r = asprintf(&message, "%c", '\0');

        va_list list;
        va_start(list, format);
        r = vasprintf(&message, format, list);
        if (r == -1) {
                perror("vasprintf message");
                exit(EXIT_FAILURE);
        }
        va_end(list);

        time(&rawtime);
        tm = localtime(&rawtime);

#ifdef WITH_MATH
        gettimeofday(&tv, NULL);
        msec = lrint(tv.tv_usec / 1000.0);
        if (msec >= 1000)
                msec -= 1000;
#else
        msec = 0;
#endif

        strftime(date_str, 80, logger->format_date, tm);
        strftime(time_str, 80, logger->format_time, tm);

        buffer = NULL;
        r = asprintf(&buffer, "%c", '\0');

        for (p = logger->format; *p != '\0'; p++) {
                tmp = buffer;
                if (*p == '%') {
                        switch(*(p+1)) {
                        case '%':
                                r = asprintf(&buffer, "%s%%%%", buffer);
                                break;
                        case 'D':
                                r = asprintf(&buffer, "%s%s", buffer, date_str);
                                break;
                        case 'T':
                                r = asprintf(&buffer, "%s%s", buffer, time_str);
                                break;
                        case 'X':
                                r = asprintf(&buffer, "%s%03d", buffer, msec);
                                break;
                        case 'N':
                                r = asprintf(&buffer, "%s%s", buffer, logger->progname);
                                break;
                        case 'L':
                                r = asprintf(&buffer, "%s%s", buffer, strloglevel(level));
                                break;
                        case 'f':
                                r = asprintf(&buffer, "%s%s", buffer, path);
                                break;
                        case 'm':
                                r = asprintf(&buffer, "%s%s", buffer, func);
                                break;
                        case 'l':
                                r = asprintf(&buffer, "%s%d", buffer, line);
                                break;
                        case 'M':
                                r = asprintf(&buffer, "%s%s", buffer, message);
                                break;
                        default:
                                fprintf(stderr, "log.c: invalid conversion specifier '%c'\n", *(p+1));
                                fflush(stderr);
                                exit(EXIT_FAILURE);
                        }

                        if (r == -1) {
                                perror("asprintf");
                                exit(EXIT_FAILURE);
                        }

                        p++;
                } else {
                        r = asprintf(&buffer, "%s%c", buffer, *p);
                }

                if (r != -1)
                        free(tmp);
        }
        current = logger->appenders;
        do {
                if (current->append != NULL)
                        current->append(buffer);
        } while((current = current->next) != NULL);

        free(buffer);
        free(message);

        return;
}

static char *strloglevel(enum log_level level)
{
        switch (level) {
        case LINFO:
                return "INFO";
        case LWARNING:
                return "WARN";
        case LERROR:
                return "ERROR";
        case LCRITICAL:
                return "CRITICAL";
        case LFATAL:
                return "FATAL";
        case LDEBUG:
                return "DEBUG";
        default:
                return "UNKNOWN";
        }
}
