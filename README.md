# tmpl

**tmpl** - Generates [mkstemp(3)](https://man.openbsd.org/mkstemp) file and writes output of `PROGRAM` to it

## Help
```
tmpl 0.1 Copyright (C) 2018 Konrad Lother <k@hiddenbox.org>

This software is supplied WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. This is free
software, and you are welcome to redistribute it under certain conditions; see
the template COPYING for details.

Generates a temp file in mkstemp(3) format and writes output of PROGRAM to it.
Optionally prints buffer to STDOUT instead of creating temp file.

Usage: tmpl [-h|--help] [--detailed-help] [--full-help] [-V|--version]
         [-f|--force] [-eKEY=VALUE|--env=KEY=VALUE]
         [-dSECONDS|--delete=SECONDS] [-pPROGRAM|--program=PROGRAM] [-c|--cat]
         [-rCOMMAND|--run=COMMAND] [-B|--background] [--stdout=FILE]
         [--stderr=FILE] [FILES]...

  -h, --help                    Print help and exit
      --detailed-help           Print help, including all details and hidden
                                  options, and exit
      --full-help               Print help, including hidden options, and exit
  -V, --version                 Print version and exit


  -f, --force                   Force output generation  (default=off)
  -T, --mkstemp-template=FORMAT Set mkstemp(3) template
                                  (default=`/tmp/.tmpl-XXXXXXXXXX')
  See mkstemp(3) man page
  -e, --env=KEY=VALUE           Set environment variable KEY to VALUE prior to
                                  running PROGRAM or COMMAND
  -d, --delete=SECONDS          Spawns new process which deletes temp file
                                  after SECONDS seconds
  -p, --program=PROGRAM         Run PROGRAM for each given FILE(S)
                                  (default=`/bin/sh')
  Example: tmpl -r "neomutt -F %f" -p "erb -T-" ~/.mutt.tmpl.erb



  -c, --cat                     Print buffer to STDOUT and exit (does not write
                                  temp file)  (default=off)

  -r, --run=COMMAND             Run COMMAND and delete temp file on exit
  Instead of returning the path, tmpl runs COMMAND
  and deletes the temp file after COMMAND returns.
  It then exits with the return code of COMMAND.

  Example: tmpl -r "neomutt -F %f" ~/.mutt.tmpl.sh

  Variables:
          %f  - The generated temp file path

  -B, --background              Fork to background when used with -r
                                  (default=off)
      --stdout=FILE             Redirect STDOUT from COMMAND to FILE
      --stderr=FILE             Redirect STDERR from COMMAND to FILE

See tmpl(1) for more informations and examples.
Send bug reports to bugs+tmpl@hiddenbox.org

```

## Manpage
```
tmpl(1)			    General Commands Manual		       tmpl(1)

NAME
     tmpl  Generates mkstemp(3) file and writes output of PROGRAM to it

SYNOPSIS
     tmpl [-hV] [-fc] [-T FORMAT] [-e KEY=VALUE] [-d SECONDS] [-p PROGRAM]
	  [-r COMMAND] template...

DESCRIPTION
     Generates mkstemp(3) file and writes output of PROGRAM to it.
     Alternativley, when used with -c, prints generated content to STDOUT and
     does not create and return a mkstemp(3) file.  mkstemp(3) files are
     created with mode 0600 by mkstemp(3) and set to 0400 via chmod(2) after
     writing STDOUT. Use -m, --mode MODE to change that.

     This utility is especially useful if you need to dynamically create
     configuration files for programs.

     tmpl does not have any templating language itself, instead it uses STDOUT
     from whatever you pass to -p PROGRAM.  This allows you to use any
     templating language you want, if it has an interpreter or if you can wrap
     a script around it.

     To use, for example, erb you'd do something like this:

	   tmpl -p /usr/local/bin/erb -T- /path/to/your.erb

OPTIONS
     -h, --help
	     Print help and exit

     --detailed-help
	     Print help, including all details and hidden options, and exit

     -V, --version
	     Print version and exit

     -f, --force
	     Force output generation even if PROGRAM fails on a template...
	     Use with caution!

     -c, --cat
	     Print output to STDOUT and do not create and return mkstemp(3)
	     file

     -T, --mkstemp-template FORMAT
	     Set mkstemp(3) template. (default: /tmp/.tmpl-XXXXXXXXXX)

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

EXAMPLES
     A typical use case would be running (neo)mutt with multiple accounts
     using a single configuration file.

	   tmpl -r 'neomutt -F %f' -e ACCOUNT=k@hiddenbox.org
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

	   neomutt -F $(tmpl -d 5 -e ACCOUNT=k@hiddenbox.org
	   ~/.mutt/mutt-gen-config.sh)

     will give neomutt 5 seconds (via -d) to read the config from mkstemp(3)
     file, which gets then deleted.

     Try the following example in your terminal:

	   cat $(tmpl -p /bin/echo 'this is a test')

     or
	   tmpl -r 'cat %f' -p /bin/echo 'this is still a test'

     This should give you a basic overview of what tmpl can do for you.

BUGS
     The parser for -r is pretty bad so don't try to use any fancy shell
     syntax with quotes or other special chars. It might lead to unexpected
     behaviour. You should instead use a wrapper script.

SEE ALSO
     mkstemp(3)

AUTHORS
     tmpl is developed by Konrad Lother <k@hiddenbox.org>.

OpenBSD 6.3			 May 28, 2018			   OpenBSD 6.3

```
