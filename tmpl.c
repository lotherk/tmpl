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
#include <stdarg.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>

#include "cmdline.h"

static char *mkstemp_template, *template_buffer;
static struct gengetopt_args_info args;

static int arg_init(int argc, char **argv);
static int tmpl_quit(int code);
static void tmpl_setenv();
static void tmpl_atexit();
static int write_mkstemp();
static int run_program(const char *arg);
static int run_command();

int main(int argc, char **argv)
{
        int r, i;
        pid_t pid;

        mkstemp_template = NULL;

        if ((r = arg_init(argc, argv)) != 0)
                exit(EXIT_FAILURE);

        if (args.background_flag) {
                pid = fork();
                if (pid > 0) {
                        exit(EXIT_SUCCESS);
                } else if (pid < 0) {
                        perror("daemonize");
                        exit(EXIT_FAILURE);
                }
        }

        tmpl_setenv();

        r = asprintf(&template_buffer, "%c", '\0');
        if (r == -1) {
                perror("template_buffer");
                exit(EXIT_FAILURE);
        }

        if (args.inputs_num == 1)
                run_program(args.inputs[0]);
        else if(args.inputs_num > 1)
                for (i = 0; i < args.inputs_num; i++)
                        run_program(args.inputs[i]);
        else
                run_program(NULL);

        if (args.cat_flag) {
                printf("%s", template_buffer);
                return tmpl_quit(0);
        }

        r = write_mkstemp();
        if (r != 0) {
                perror("write_mkstemp");
                exit(EXIT_FAILURE);
        }

        if (args.run_given) {
                int code;
                code = run_command();

                if (code < 0) {
                        perror("run_command");
                        exit(EXIT_FAILURE);
                }

                r = unlink(mkstemp_template);

                if (r != 0)
                        perror("unlink mkstemp_template");

                return tmpl_quit(code);
        }

        fprintf(stdout, "%s\n", mkstemp_template);

        if (args.delete_given) {
                pid = fork();

                if (pid > 0) {
                        exit(tmpl_quit(0));
                } else if (pid < 0) {
                        perror("fork for delete");
                        exit(EXIT_FAILURE);
                }
                daemon(0, 0);

                sleep(args.delete_arg);
                r = unlink(mkstemp_template);
                exit(tmpl_quit(r));
        }

        return tmpl_quit(0);
}

static int arg_init(int argc, char **argv)
{
        int r;

        if((r = cmdline_parser(argc, argv, &args)) != 0)
                exit(EXIT_FAILURE);


        return 0;
}

static void tmpl_setenv()
{
        int i, r;
        const char *delim = "=";
        for (i = 0; i < args.environment_given; i++) {
                char *key, *value;
                key = strtok_r(args.environment_arg[i], delim, &value);
                if (key == NULL || value == NULL)
                        continue;
                r = setenv(key, value, 1);
        }
}
static void tmpl_atexit()
{
        tmpl_quit(1);
}
static int tmpl_quit(int code)
{
        free(template_buffer);
        free(mkstemp_template);
        return code;
}

static int run_program(const char *arg)
{
        int r;
        char *path, *buf, *tmp;
        char line[PATH_MAX];

        FILE *fp;

        r = asprintf(&buf, "%c", '\0');
        if (r == -1) {
                perror("run_program buf");
                exit(EXIT_FAILURE);
        }

        if (arg == NULL) {
                path = args.program_arg;
        } else {
                r = asprintf(&path, "%s %s", args.program_arg, arg);
                if (r == -1) {
                        perror("run_program asprintf");
                        exit(EXIT_FAILURE);
                }
        }

        fp = popen(path, "r");

        if (fp == NULL) {
                perror("popen");
                exit(EXIT_FAILURE);
        }

        while (fgets(line, PATH_MAX, fp) != NULL) {
                tmp = buf;
                r = asprintf(&buf, "%s%s", buf, line);
                if (r == -1) {
                        perror("fgets asprintf");
                        exit(EXIT_FAILURE);
                }
                free(tmp);
        }

        r = pclose(fp);
        if (r == -1) {
                perror("run_program pclose");
                exit(EXIT_FAILURE);
        }

        r = WEXITSTATUS(r);

        if (r != 0 && !args.force_flag) {
                free(buf);
                return r;
        }

        r = asprintf(&template_buffer, "%s%s", template_buffer, buf);
        if (r == -1) {
                perror("run_program template_buffer");
                exit(EXIT_FAILURE);
        }
        free(buf);
        return 0;
}

static int write_mkstemp()
{
        int fd, r;
        size_t size;

        mkstemp_template = strdup(args.mkstemp_template_arg);
        if (mkstemp_template == NULL) {
                perror("mkstemp_template");
                exit(EXIT_FAILURE);
        }

        fd = mkstemp(mkstemp_template);
        if (fd == -1) {
                perror("mkstemp");
                exit(EXIT_FAILURE);
        }

        size = strlen(template_buffer);
        r = write(fd, template_buffer, size);
        if (r != size) {
                perror("write");
                exit(EXIT_FAILURE);
        }
        r = fchmod(fd, 0400);
        if (r == -1) {
                perror("fchmod");
                exit(EXIT_FAILURE);
        }
        r = close(fd);
        if (r != 0) {
                perror("close");
                exit(EXIT_FAILURE);
        }
        return 0;
}

static void run_command_child()
{

	char *ptr, *arg, *program, *command;
	char *delim = " ";
	char *arguments[PATH_MAX];
	size_t arguments_i = 0;
	char abuf[PATH_MAX] = { '\0' };
	size_t abuf_i;
	int i;

        command = args.run_arg;


	arg = NULL;
	program = strtok_r(command, delim, &arg);

	if (NULL == program)
		program = command;

	arguments[arguments_i++] = program;

	if (NULL != arg)
	for (ptr = arg; *ptr != '\0'; ptr++) {

		if (*ptr == '%') {
			ptr++;
			switch (*ptr) {
				case '%':
					break;

				case 'f':
					arguments[arguments_i++] = mkstemp_template;
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
        return;
}
static int run_command()
{

        int stdin_pipe[2], stdout_pipe[2], stderr_pipe[2];
        FILE *cstdin, *cstdout, *cstderr;

        pid_t pid, wpid, bpid;
        int status, r;

        FILE *redir_stdout = stdout;
        FILE *redir_stderr = stderr;


        pipe(stdin_pipe);
        pipe(stdout_pipe);
        pipe(stderr_pipe);

        pid = fork();

        if (pid == 0) {
                close(stdin_pipe[1]);
                close(stdout_pipe[0]);
                close(stderr_pipe[0]);

                dup2(stdin_pipe[0], STDIN_FILENO);
                dup2(stdout_pipe[1], STDOUT_FILENO);
                dup2(stderr_pipe[1], STDERR_FILENO);

                run_command_child();
        } else if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
        } else {

                if (args.stdout_given) {
                        redir_stdout = fopen(args.stdout_arg, "w");
                        if (redir_stdout == NULL) {
                                perror("redir_stdout");
                                exit(EXIT_FAILURE);
                        }
                }

                if (args.stderr_given) {
                        redir_stderr = fopen(args.stderr_arg, "w");
                        if (redir_stderr == NULL) {
                                perror("redir_stderr");
                                exit(EXIT_FAILURE);
                        }
                }

                close(stdin_pipe[0]);
                close(stdout_pipe[1]);
                close(stderr_pipe[1]);

                cstdin = fdopen(stdin_pipe[1], "w");
                if (cstdin == NULL) {
                       perror("cstdin");
                       exit(EXIT_FAILURE);
                }

                cstdout = fdopen(stdout_pipe[0], "r");
                if (cstdout == NULL) {
                        perror("cstdout");
                        exit(EXIT_FAILURE);
                }

                cstderr = fdopen(stderr_pipe[0], "r");
                if (cstderr == NULL) {
                        perror("cstderr");
                        exit(EXIT_FAILURE);
                }

                int do_read = 1;
                int od = 0;
                int ed = 0;
                int i;
                int c;
                int s;


                do {
                        if (ioctl(stderr_pipe[0], FIONREAD, &s) == 0 && s > 0) {
                                for ( i = 0; i < s; i ++) {
                                        c = fgetc(cstderr);
                                        fputc(c, redir_stderr);
                                }
                        }
                        if (ioctl(stdout_pipe[0], FIONREAD, &s) == 0 && s > 0) {
                                for ( i = 0; i < s; i ++) {
                                        c = fgetc(cstdout);
                                        fputc(c, redir_stdout);
                                }
                        }
                        waitpid(pid, &status, WNOHANG);
                        if (WIFEXITED(status)) {
                                do_read = 0;
                                break;
                        }

                        sleep(1);
                } while (do_read != 0);

                fflush(cstdout);
                fflush(cstderr);
                fclose(cstdin);
                fclose(cstdout);
                fclose(cstderr);

                if (redir_stdout != stdout)
                        fclose(redir_stdout);

                if (redir_stderr != stderr)
                        fclose(redir_stderr);

                if (WIFEXITED(status))
                        return WEXITSTATUS(status);
        }

        return -1; // this should never be reached
}
