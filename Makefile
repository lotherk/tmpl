PREFIX := /usr/local
CFLAGS := -g
default: all

all: program

opts: cmdline
cmdline:
	gengetopt -u -i tmpl.ggo

program:
	cc -g -o tmpl tmpl.c log.c cmdline.c
readme.md: all
	./tmpl -c -p "erb -T-" ./README.md.erb > README.md
static:
	cc -g -o tmpl-static -static tmpl.c log.c cmdline.c

clean:
	-rm -f tmpl
	-rm -f tmpl-static

install:
	install -d ${PREFIX}/bin
	install -m 755 tmpl ${PREFIX}/bin/
	install -d ${PREFIX}/share/man/man1/
	install -m 644 tmpl.1  ${PREFIX}/share/man/man1/

deinstall:
	-rm -f ${PREFIX}/bin/tmpl
	-rm -f ${PREFIX}/share/man/man1/tmpl.1

gdb:
	make cmdline all
	gdb ./tmpl
