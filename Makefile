PREFIX := /usr/local
CFLAGS := -g
MANDIR := ${PREFIX}/man/man1/
C := cc
UNAME_S = `uname -s`
UNAME_M = `uname -m`

default: all

all: program

opts: cmdline
cmdline:
	gengetopt -u -i tmpl.ggo

program:
	${C} ${CFLAGS} -o tmpl tmpl.c cmdline.c
readme.md: all
	./tmpl -c -p "erb -T-" ./README.md.erb > README.md
static:
	${C} -o tmpl-static-${UNAME_S}-${UNAME_M} -static tmpl.c cmdline.c
	strip --strip-all tmpl-static-${UNAME_S}-${UNAME_M}
clean:
	-rm -f tmpl
	-rm -f tmpl-static-${UNAME_S}-${UNAME_M}

install:
	install -d ${PREFIX}/bin
	install -m 755 tmpl ${PREFIX}/bin/
	strip --strip-all ${PREFIX}/bin/tmpl
	install -m 644 tmpl.1  ${MANDIR}/

deinstall:
	-rm -f ${PREFIX}/bin/tmpl
	-rm -f ${MANDIR}/tmpl.1

gdb:
	make cmdline all
	gdb ./tmpl
