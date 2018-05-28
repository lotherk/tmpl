# tmpl



## Manpage
```

NAME
     tmpl – Generates mkstemp(3) file and writes output of PROGRAM to it

SYNOPSIS
     tmpl [-hV] [-fc] [-T FORMAT] [-e KEY=VALUE] [-d SECONDS] [-p PROGRAM]
	  [-r COMMAND] template...

DESCRIPTION
     Generates mkstemp(3) file and writes output of PROGRAM to it.
     Alternativley, when used with -c, prints generated content to STDOUT and
     does not create and return a mkstemp(3) file.

     This utility is especially useful if you need to dynamically create
     configuration files for programs.

     A typical use case would be running (neo)mutt with multiple accounts
     using a single configuration file.

	   tmpl -r 'neomutt -F %f' \
	   -e ACCOUNT=k@hiddenbox.org \
	   ~/.mutt/mutt-gen-config.sh

     This will pass ~/.mutt/mutt-gen-config.sh, which is a script that
     generates a mutt configuration file, to /bin/sh. All output mutt-gen-
     config.sh generates will be written to a mkstemp(3) file which is then
     read by neomutt. Since neomutt is called by tmpl via -r, tmpl
     automatically deletes the generated mkstemp(3) file as soon as neomutt
     exits.

     The same example could also be written as:

	   neomutt -F $(tmpl -e ACCOUNT=k@hiddenbox.org
	   ~/.mutt/mutt-gen-config.sh)

     but in this case tmpl can not remove the mkstemp(3) file since it never
     knows when neomutt exits. A better approach would be something like

	   CFG=$(tmpl -e ACCOUNT=k@hiddenbox.org ~/.mutt/mutt-gen-config.sh)
	   neomutt -F $CFG; rm -f $CFG

     or

	   neomutt -F $(tmpl -d 5 -e ACCOUNT=k@hiddenbox.or
	   ~/.mutt/mutt-gen-config.sh)

     will give neomutt 5 seconds (via -d) to read the config from mkstemp(3)
     file, which gets then deleted.

     Try the following example in your terminal:

	   cat $(tmpl -p /bin/echo 'this is a test')

     or
	   tmpl -r 'cat %f' -p /bin/echo 'this is still a test'

     This should give you a basic overview of what tmpl can do for you.

OPTIONS
     -h, --help
	     Print help and exit

     --detailed-help
	     Print help, including all details and hidden options, and exit

     -V, --version
	     Print version and exit

     -f, --force
	     Force output generation even if PROGRAM failes on a template...
	     Use with caution!

     -c, --cat
	     Print output to STDOUT and do not create and return mkstemp(3)
	     file

     -T, --mkstemp-template FORMAT
	     Set mkstemp(3) template. (default: /tmp/.tmpl-XXXXXX)

     -e, --environment KEY=VALUE
	     Set environment variable KEY to VALUE prior to running PROGRAM or
	     COMMAND

     -d, --delete SECONDS
	     Spawns new process which deletes mkstemp(3) file after SECONDS
	     seconds. Value is passed to sleep(3).

     -r, --run COMMAND
	     Run COMMAND and deletes mkstemp(3) file afterwards

     -p, --program PROGRAM
	     Pass template... to PROGRAM (default: /bin/sh)

TEMPLATE
     A TEMPLATE is a shell script or any other input file or argument given to
     PROGRAM set with -p.

     The default program is /bin/sh

     For example:

	   tmpl -p /bin/sh /path/to/script1.sh /path/to/script2.sh

     will run script1.sh and script2.sh successively and result in 2 system
     calls:

	   /bin/sh /path/to/script1.sh
	   /bin/sh /path/to/script2.sh

     If either script fails to execute no output or mkstemp(3) file will be
     generated unless -f is used.

     For example:

	   tmpl -f -r 'httpd -f %f' server.sh vhosts.sh

     will run httpd with the generated mkstemp(3) file as its configuration
     file even though one of the scripts might have failed. This could lead to
     an unsafe httpd config or otherwise unwanted behaviour. The use of -f is
     not encouraged unless you are really sure!

     STDERR is never written to tmpl its buffer.

BUGS
     The parser for -r is pretty bad so don't try to use any fancy shell
     syntax with quotes or other special chars. It might lead to unexpected
     behaviour. You should instead use a wrapper script.

SEE ALSO
     mkstemp(3)

AUTHORS
     tmpl is developed by Konrad Lother <k@hiddenbox.org>.

FreeBSD 12.0-CURRENT		 May 28, 2018		  FreeBSD 12.0-CURRENT

```
