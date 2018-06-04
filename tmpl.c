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
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/filio.h>
#include <limits.h>
#include <unistd.h>
#include "cmdline.h"

static char *mkstemp_template, *template_buffer;
static struct gengetopt_args_info args;
static int run_command_exit_code;

static int arg_init(int argc, char **argv);
static int set_environment();
static void atexit_hook();
static int write_mkstemp();
static int run_program(const char *arg);
static int run_command();

int main(int argc, char **argv)
{
    int r, i;
    pid_t pid;
    char *tmp;

    mkstemp_template = NULL;
    template_buffer = NULL;

    run_command_exit_code = 0;

    if ((r = arg_init(argc, argv)) != 0)
        exit(EXIT_FAILURE);

    if (args.background_flag) {
        if ((r = daemon(0, 0)) != 0) {
            perror("daemonize");
            exit(EXIT_FAILURE);
        }
    }

    if((r = set_environment()) != 0) {
        perror("set_environment");
        exit(EXIT_FAILURE);
    }

    tmp = template_buffer;
    if((r = asprintf(&template_buffer, "%c", '\0')) == -1) {
        perror("template_buffer");
        exit(EXIT_FAILURE);
    }
    free(tmp);

    if (args.inputs_num == 1)
        r = run_program(args.inputs[0]);
    else if(args.inputs_num > 1)
        for (i = 0; i < args.inputs_num; i++) {
            r = run_program(args.inputs[i]);

            if (r != 0 && !args.force_flag) {
                perror("run_program");
                exit(EXIT_FAILURE);
            }
        }
    else
        r = run_program(NULL);

    if (r != 0 && !args.force_flag) {
        perror("run_program");
        exit(EXIT_FAILURE);
    }

    if (args.cat_flag) {
        fprintf(stdout, "%s", template_buffer);
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }

    if ((r = write_mkstemp()) != 0) {
        perror("write_mkstemp");
        exit(EXIT_FAILURE);
    }

    if (args.run_given) {
        if ((r = run_command()) != 0) {
            perror("run_command");
            exit(EXIT_FAILURE);
        }

        if ((r = unlink(mkstemp_template)) != 0)
            perror("unlink mkstemp_template");

        exit(run_command_exit_code);
    }


    if (args.delete_given) {
        pid = fork();

        if (pid == 0) {
            if((r = daemon(0, 0)) == -1) {
                perror("delete daemon");
                exit(EXIT_FAILURE);
            }

            sleep(args.delete_arg);
            r = unlink(mkstemp_template);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            perror("fork for delete");
            exit(EXIT_FAILURE);
        }
    }

    fprintf(stdout, "%s\n", mkstemp_template);
    fflush(stdout);
    return EXIT_SUCCESS;
}

static int arg_init(int argc, char **argv)
{
    int r;

    if((r = cmdline_parser(argc, argv, &args)) != 0)
        return 1;


    if (!args.run_given &&
            (args.stdout_given || args.stderr_given || args.background_flag)) {
        fprintf(stderr, "tmpl: -r required, see --help\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return 0;
}

static int set_environment()
{
    int i, r;
    const char *delim = "=";

    for (i = 0; i < args.env_given; i++) {
        char *key, *value;

        key = strtok_r(args.env_arg[i], delim, &value);
        if (key == NULL || value == NULL)
            continue;

        if((r = setenv(key, value, 1)) != 0)
            return 1;
    }

    return 0;
}
static void atexit_hook()
{
    if (template_buffer != NULL)
        free(template_buffer);

    if (mkstemp_template != NULL)
        free(mkstemp_template);
}

static int run_program(const char *arg)
{
    int r;
    char *path, *buf, *tmp;
    char line[PATH_MAX];
    FILE *fp;

    buf = calloc(1, sizeof(char));
    if (buf == NULL)
        return 1;

    if (arg == NULL) {
        path = args.program_arg;
    } else {
        tmp = path;
        r = asprintf(&path, "%s %s", args.program_arg, arg);
        free(tmp);

        if (r == -1)
            return 1;
    }

    fp = popen(path, "r");
    if (fp == NULL)
        return 1;

    while (fgets(line, PATH_MAX, fp) != NULL) {
        tmp = buf;
        r = asprintf(&buf, "%s%s", buf, line);
        free(tmp);

        if (r == -1)
            return 1;
    }

    r = pclose(fp);
    if (r == -1)
        return 1;

    r = WEXITSTATUS(r);

    if (r != 0 && !args.force_flag) {
        free(buf);
        return 1;
    }

    tmp = template_buffer;
    r = asprintf(&template_buffer, "%s%s", template_buffer, buf);
    free(tmp);

    free(buf);

    return r == 0 ? 0 : 1;
}

static int write_mkstemp()
{
    int fd, r;
    size_t size;

    mkstemp_template = strdup(args.mkstemp_template_arg);
    if (mkstemp_template == NULL)
        return 1;

    fd = mkstemp(mkstemp_template);
    if (fd == -1)
        return 1;

    size = strlen(template_buffer);
    if ((r = write(fd, template_buffer, size)) != size)
        return 1;

    if ((r = fchmod(fd, 0400)) == -1)
        return 1;

    if ((r = close(fd)) != 0)
        return 1;

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

    if (program == NULL)
        program = command;

    arguments[arguments_i++] = program;

    if (arg != NULL)
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

    execvp(program, arguments);
    perror("execvp");
    exit(EXIT_FAILURE);
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
        return 1;
    } else {
        if (args.stdout_given)
            redir_stdout = fopen(args.stdout_arg, "w");

        if (redir_stdout == NULL)
                return 1;

        if (args.stderr_given)
            redir_stderr = fopen(args.stderr_arg, "w");

        if (redir_stderr == NULL)
            return 1;

        close(stdin_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        cstdin = fdopen(stdin_pipe[1], "w");
        if (cstdin == NULL)
            return 1;

        cstdout = fdopen(stdout_pipe[0], "r");
        if (cstdout == NULL)
            return 1;

        cstderr = fdopen(stderr_pipe[0], "r");
        if (cstderr == NULL)
            return 1;

        int i;
        size_t s;

        do {
            if (ioctl(stderr_pipe[0], FIONREAD, &s) == 0 && s > 0)
                for ( i = 0; i < s; i ++)
                    fputc(fgetc(cstderr), redir_stderr);

            if (ioctl(stdout_pipe[0], FIONREAD, &s) == 0 && s > 0)
                for ( i = 0; i < s; i ++)
                    fputc(fgetc(cstdout), redir_stdout);

            waitpid(pid, &status, WNOHANG);
            if (WIFEXITED(status))
                break;

            sleep(1);
        } while (1);

        fflush(cstdout);
        fflush(cstderr);
        fclose(cstdin);
        fclose(cstdout);
        fclose(cstderr);

        if (redir_stdout != stdout)
            fclose(redir_stdout);

        if (redir_stderr != stderr)
            fclose(redir_stderr);

        if (! WIFEXITED(status))
            return 1;

        run_command_exit_code = WEXITSTATUS(status);
        return 0;
    }

    perror("run_command");
    return 1; // this should never be reached
}
