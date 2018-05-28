/* tmpl.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>

#include "cmdline.h"

#define FILE_MODE 0600

static char *template = NULL;
static char *buf = NULL;

static void _perrorf(int quit, const char *format, ...);
static void _setenv(char **env, size_t given);
static int _quit(int code);
static void run_command(char *command);
static void delete_mkstemp(float seconds);

int main(int argc, char **argv)
{
	int i, r, tfd, code;
	size_t size;

	pid_t pid, wpid;

	struct gengetopt_args_info args_info;
	size_t buf_i = 0;

	char *run_path;
	FILE *fp;

	r = cmdline_parser(argc, argv, &args_info);

	if (0 != r)
		_perrorf(EXIT_FAILURE, "failed to parse command line arguments");
	if (0 == args_info.inputs_num)
		_perrorf(EXIT_FAILURE, "see --help");

	template = strdup(args_info.mkstemp_template_arg);
	if (NULL == template)
		_perrorf(EXIT_FAILURE, "malloc template");

	tfd = mkstemp(template);
	if (-1 == tfd)
		_perrorf(EXIT_FAILURE, "mkstemp %s", template);

	if (0 < args_info.environment_given) {
		_setenv(args_info.environment_arg, args_info.environment_given);
	}


	for (i = 0; i < args_info.inputs_num; i++) {
		char line[PATH_MAX];
		char *lbuf = NULL;
		size_t lbuf_i = 0;
		size = (strlen(args_info.program_arg) + 1 + strlen(args_info.inputs[i]) + 1);
		run_path = malloc(sizeof(char) * size);
		snprintf(run_path, size, "%s %s", args_info.program_arg, args_info.inputs[i]);

		fp = popen(run_path, "r");

		if (NULL == fp) {
			_perrorf(0, "popen %s", run_path);
			goto clean;
		}

		while (fgets(line, PATH_MAX, fp) != NULL) {
			size = strlen(line);

			if (NULL == lbuf) {
				lbuf = calloc(size, sizeof(char));
				if (NULL == lbuf)
					_perrorf(EXIT_FAILURE, "malloc lbuf");
			} else {
				lbuf = realloc(lbuf, sizeof(char) * (buf_i + size));
				if (NULL == lbuf)
					_perrorf(EXIT_FAILURE, "realloc lbuf");
			}
			buf_i += size;
			strncat(lbuf, line, strlen(line));
		}

		r = pclose(fp);
		if (-1 == r) {
			_perrorf(0, "pclose %s", run_path);
			goto clean;
		}

		r = WEXITSTATUS(r);

		if (0 != r) {
			if (0 != args_info.force_flag)
				code = 0;
			else
				code = EXIT_FAILURE;

			_perrorf(code, "%s terminated non-zero %d", run_path, r);

			if (0 != args_info.force_flag)
				goto clean;
		}

		if (lbuf == NULL)
			goto clean;

		size = strlen(lbuf);
		if (NULL == buf) {
			buf = calloc(strlen(lbuf) + 1, sizeof(char));
			if (NULL == buf)
				_perrorf(EXIT_FAILURE, "malloc buf");

		} else {
			buf = realloc(buf, sizeof(char) * (buf_i + strlen(lbuf) + 1));
			if (NULL == buf)
				_perrorf(EXIT_FAILURE, "realloc buf");
		}


		strncat(buf, lbuf, size);
		buf_i += size;

clean:
		if (NULL != lbuf) {
			free(lbuf);
			lbuf = NULL;
		}

		if (NULL != run_path) {
			free(run_path);
			run_path = NULL;
		}
	}

	if (NULL == buf)
		goto quit;

	if (0 == strlen(buf))
		goto quit;

	if (args_info.cat_flag) {
		fprintf(stdout, "%s", buf);
		goto quit;
	}

	r = write(tfd, buf, strlen(buf));

	if (r != strlen(buf))
		_perrorf(EXIT_FAILURE, "write (%d/%d)", r, strlen(buf));

	r = close(tfd);
	if (0 != r)
		_perrorf(EXIT_FAILURE, "close");

	r = chmod(template, 0400);
	if (0 != r)
		_perrorf(EXIT_FAILURE, "chmod");

	if (args_info.run_given) {
		pid = fork();

		if (pid == 0) { // child
			 run_command(args_info.run_arg);
		} else if (pid <0) { // fork failed
			_perrorf(EXIT_FAILURE, "failed to fork");
		} else { // parent
			int status = 0;
			while ((wpid = wait(&status)) > 0);
			r = unlink(template);
			if (0 != r)
				_perrorf(0, "could not unlink %s", template);

			if (WIFEXITED(status))
				_quit(WEXITSTATUS(status));
		}
	} else {
		fprintf(stdout, "%s\n", template);
	}
quit:

	if (NULL != buf)
		free(buf);


	fflush(stdout);

	if (args_info.delete_given)
		delete_mkstemp(args_info.delete_arg);

	return _quit(0);
}


static int _quit(int code)
{
	if (NULL != template)
		free(template);

	exit(code);
}


static void _perrorf(int q, const char *format, ...)
{
	char buf[PATH_MAX];
	va_list list;

	va_start(list, format);
	vsnprintf(buf, PATH_MAX, format, list);
	va_end(list);
	if (0 != errno) {
		perror(buf);
	} else {
		fprintf(stderr, "%s\n", buf);
		fflush(stderr);
	}

	if (0 != q) {
		fflush(stdout);
		_quit(q);
	}
}

static void _setenv(char **env, size_t given)
{
	int i, r;

	for (i = 0; i < given; i++) {
		char *saveptr;
		char *key, *value;
		const char *delim = "=";
		const char *delim1 = "\0";

		key = strtok_r(env[i], delim, &saveptr);
		if (NULL == key)
			continue;

		value = strtok_r(NULL, delim1, &saveptr);
		if (NULL == key)
			continue;


		r = setenv(key, value, 1);
	}
}

static void run_command(char *command)
{
	char *ptr, *args, *program;
	char *delim = " ";
	char *delim1 = "";
	char *arguments[PATH_MAX];
	size_t arguments_i = 0;
	char abuf[PATH_MAX] = { '\0' };
	size_t abuf_i;
	int i;

	args = NULL;
	program = strtok_r(command, delim, &args);

	if (NULL == program)
		program = command;

	arguments[arguments_i++] = program;

	if (NULL != args)
	for (ptr = args; *ptr != '\0'; ptr++) {

		if (*ptr == '%') {
			ptr++;
			switch (*ptr) {
				case '%':
					break;

				case 'f':
					arguments[arguments_i++] = template;
					continue;
				default:
					break;
			}
		}

		if (*ptr == ' ' || *ptr == '\0') {
			if (abuf_i > 0) {
				abuf[abuf_i++] = '\0';
				arguments[arguments_i++] = strdup(abuf);
				for (i = 0; i < abuf_i; i++)
					abuf[i] = '\0';
				abuf_i = 0;
			}
			continue;
		} else {
			abuf[abuf_i++] = *ptr;
		}
	}

	arguments[arguments_i] = NULL;

	i = execvp(program, arguments);
	// execvp failed
}

static void delete_mkstemp(float seconds) {
	_perrorf(EXIT_FAILURE, "--delete is not yet implemented!");
}
