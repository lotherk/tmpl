PREFIX := /usr/local

default: all

all: program

opts: cmdline
cmdline:
	gengetopt -u -i tmpl.ggo

program:
	cc -o tmpl tmpl.c cmdline.c
readme.md: all
	./tmpl -c -p "erb -T-" ./README.md.erb > README.md
	make clean
static:
	cc -o tmpl-static -static tmpl.c cmdline.c

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
