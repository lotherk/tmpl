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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>
#include <err.h>


#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include "cmdline.h"
#include "rstrcat.h"

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

    mkstemp_template = NULL;
    template_buffer = NULL;
    run_command_exit_code = 0;

#ifdef HAVE_PLEDGE
    if (pledge("error stdio fattr proc exec tmppath rpath wpath cpath", NULL) == -1)
        err(1, "pledge");
#endif

    atexit(atexit_hook);

    if ((r = arg_init(argc, argv)) != 0)
        err(1, "arg_init");

    if (args.background_flag)
        if ((r = daemon(0, 0)) != 0)
            err(1, "daemonize");


    if((r = set_environment()) != 0)
        err(1, "set_environment");

    if (args.inputs_num == 1)
        r = run_program(args.inputs[0]);
    else if(args.inputs_num > 1)
        for (i = 0; i < args.inputs_num; i++) {
            r = run_program(args.inputs[i]);

            if (r != 0 && !args.force_flag)
               err(1, "run_program");
        }
    else
        r = run_program(NULL);

    if (r != 0 && !args.force_flag)
        err(1, "run_program");

    if (args.cat_flag) {
        fprintf(stdout, "%s", template_buffer);
        fflush(stdout);
        exit(EXIT_SUCCESS);
    }

    if ((r = write_mkstemp()) != 0)
        err(1, "write_mkstemp (%d)", r);

    if (args.run_given) {
        if ((r = run_command()) != 0)
            err(1, "run_command");
        if ((r = unlink(mkstemp_template)) != 0)
            warn("unlink mkstemp_template");

        exit(run_command_exit_code);
    }

    if (args.delete_given) {
        pid = fork();

        if (pid == 0) {
            if((r = daemon(0, 0)) == -1)
                err(1, "delete daemon");

#ifdef HAVE_PLEDGE
            if (pledge("stdio tmppath", NULL) == -1)
                err(1, "pledge");
#endif
            sleep(args.delete_arg);
            r = unlink(mkstemp_template);
            exit(EXIT_SUCCESS);
        } else if (pid < 0) {
            err(1, "fork for delete");
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
        errx(1, "-r required, see --help\n");
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

    path = NULL;
    buf = NULL;

    if (arg == NULL) {
        path = strdup(args.program_arg);
    } else {
        tmp = path;
        r = frstrcat(&path, "%s %s", args.program_arg, arg);
        if (r == -1) {
            free(buf);
            free(path);
            return 1;
        }
        free(tmp);
    }

    fp = popen(path, "r");
    if (fp == NULL) {
        free(buf);
        free(path);
        return 1;
    }

    while (fgets(line, PATH_MAX, fp) != NULL) {
        tmp = buf;
        r = rstrcat(&buf, line);
        if (r == -1) {
            free(tmp);
            free(path);
            return 1;
        }
        free(tmp);
    }

    r = pclose(fp);
    free(path);
    if (r == -1)
        return 1;

    r = WEXITSTATUS(r);
    if (r != 0 && !args.force_flag) {
        free(buf);
        return 1;
    }

    tmp = template_buffer;
    r = rstrcat(&template_buffer, buf);
    if (r == -1) {
        template_buffer = tmp;
        tmp = NULL;
        free(buf);
        return 1;
    }
    free(tmp);
    free(buf);

    return 0;
}

static int write_mkstemp()
{
    int fd, r;
    size_t size;

    mkstemp_template = strdup(MKSTEMP_TEMPLATE);
    if (mkstemp_template == NULL)
        return -1;
    fd = mkstemp(mkstemp_template);
    if (fd == -1)
        return -2;

    if ((r = fchmod(fd, 0600)) == -1)
        return -3;

    size = strlen(template_buffer);
    if ((r = write(fd, template_buffer, size)) != size)
        return -4;

    if ((r = fchmod(fd, 0400)) == -1)
        return -5;

    if ((r = close(fd)) != 0)
        return -6;

    return 0;
}

static void run_command_child()
{

    char *ptr, *arg, *program, *command;
    char *delim = " ";
    char *arguments[PATH_MAX];
    size_t arguments_i = 0;
    char abuf[PATH_MAX] = { '\0' };
    size_t abuf_i = 0;
    int i;

#ifdef HAVE_PLEDGE
    if (pledge("exec", NULL) == -1)
        err(1, "pledge");
#endif

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
    err(1, NULL);
}
static int run_command()
{

    int stdin_pipe[2], stdout_pipe[2], stderr_pipe[2];
    FILE *cstdin, *cstdout, *cstderr;
    pid_t pid;
    int status, r;
    FILE *redir_stdout = stdout;
    FILE *redir_stderr = stderr;

    pipe(stdin_pipe);
    pipe(stdout_pipe);
    pipe(stderr_pipe);

    status = 0;
    pid = fork();
    if (pid == 0) {
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        dup2(stdin_pipe[0], STDIN_FILENO);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        run_command_child();
        err(1, "run_command_child");
    } else if (pid < 0) {
        return 1;
    } else {
        if (args.stdout_given)
            redir_stdout = fopen(args.stdout_arg, "w");

        if (redir_stdout == NULL) {
            return 1;
        }

        if (args.stderr_given) {
            redir_stderr = fopen(args.stderr_arg, "w");

            if (redir_stderr == NULL) {
                if (redir_stdout != stdout)
                    fclose(redir_stdout);
                return 1;
            }
        } else {
            redir_stderr = redir_stdout;
        }

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
        struct timespec tim;
        fd_set fds;
        int maxfd;

        FD_ZERO(&fds);
        FD_SET(stdout_pipe[0], &fds);
        FD_SET(stderr_pipe[0], &fds);

        maxfd = stderr_pipe[0] > stdout_pipe[0] ? stderr_pipe[0] : stdout_pipe[0];

        tim.tv_sec = 0;
        tim.tv_nsec = 50000000L;
        s = 1;
        while(pselect(maxfd + 1, &fds, NULL, NULL, &tim, NULL) != -1) {
            if (ioctl(stderr_pipe[0], FIONREAD, &s) == 0 && s > 0) {
                for ( i = 0; i < s; i++)
                    fputc(fgetc(cstderr), redir_stderr);
                fflush(redir_stderr);
            }
            if (ioctl(stdout_pipe[0], FIONREAD, &s) == 0 && s > 0) {
                for ( i = 0; i < s; i++)
                    fputc(fgetc(cstdout), redir_stdout);
                fflush(redir_stdout);
            }
            r = waitpid(pid, &status, WNOHANG);
            if (r != 0) {
                if (WIFEXITED(status))
                    break;

                else if (WIFSIGNALED(status))
                    err(1, "child received unhandled signal");

                else if (WCOREDUMP(status))
                    err(1, "child core dumped");

                else if (WIFSTOPPED(status))
                    err(1, "child stopped");

                else
                    errx(1, "child unknown exit");
            }
        }

        fclose(cstdin);
        fclose(cstdout);
        fclose(cstderr);
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        if (redir_stderr != stderr && redir_stderr != redir_stdout)
            fclose(redir_stderr);

        if (redir_stdout != stdout)
            fclose(redir_stdout);

        run_command_exit_code = WEXITSTATUS(status);
        return 0;
    }

    return 1; // this should never be reached
}
