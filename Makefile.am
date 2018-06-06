AUTOMAKE_OPTIONS = foreign
SUBDIRS = src man

cmdline:
	gengetopt -u -i tmpl.ggo --output-dir src/

static:
	${CC} ${CFLAGS} -static -I . -DSTATIC_BUILD -o tmpl-static-`uname -s`-`uname -m`-${VERSION} src/tmpl.c src/cmdline.c src/rstrcat.c
	strip --strip-all tmpl-static-`uname -s`-`uname -m`-${VERSION}

