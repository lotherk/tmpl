AUTOMAKE_OPTIONS = foreign
SUBDIRS = src man

cmdline:
	gengetopt -u -i tmpl.ggo --output-dir src/

static:
	${CC} ${CFLAGS} -static -I . -o tmpl-static-`uname -s`-`uname -m` src/tmpl.c src/cmdline.c
	strip --strip-all tmpl-static-`uname -s`-`uname -m`
