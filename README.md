# tmpl

**tmpl** is a small utility tool which creates [mkstemp(3)](https://man.openbsd.org/mkstemp)

## Help
```
tmpl 0.1 Copyright (C) 2018 Konrad Lother <k@hiddenbox.org>

This software is supplied WITHOUT ANY WARRANTY; without even the implied
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. This is free
software, and you are welcome to redistribute it under certain conditions; see
the template COPYING for details.

Generates mkstemp(3) file and writes output of PROGRAM to it.

Usage: tmpl [OPTIONS]... [FILES]...

  -h, --help                    Print help and exit
      --detailed-help           Print help, including all details and hidden
                                  options, and exit
  -V, --version                 Print version and exit

General:


  -l, --log                     Enable logging  (default=off)
  -L, --log-file=FILE           Write logs to FILE  (default=`/dev/stderr')
  -v, --verbose=INT             Be more verbose. Can be specified multiple
                                  times to a maximum of 7
  
  Occurances    Level
  1             Fatal
  2             Critical
  3             Error
  4             Warning
  5             Info
  6             Debug
  7             All

  Examples:

  log all Critical and thus also Fatal messages
  # tmpl -v 2 ...

  log all messages
  # tmpl -v 7 ...

  -f, --force                   Force output generation even if PROGRAM fails
                                  on a template. Use with caution!
                                  (default=off)
  If PROGRAM fails on a template, no data (from that template)
  will be added to the global buffer, instead of aborting. This might lead to
  unwanted
  behaviour if you use tmpl for config file generation.

  -c, --cat                     Print buffer to STDOUT (does not write
                                  mkstemp(3) file)  (default=off)
  -T, --mkstemp-template=FORMAT Set mkstemp(3) template.
                                  (default=`/tmp/.tmpl-XXXXXXXXXX')
  See mkstemp(3) man page
  -e, --environment=KEY=VALUE   Set environment variable ENV to VALUE prior to
                                  running PROGRAM or COMMAND
  -d, --delete=N                Spawns new process which deletes mkstemp(3)
                                  file after N seconds.
  -p, --program=PROGRAM         Pass templateN to PROGRAM.  (default=`/bin/sh')
  Example: tmpl -p /usr/local/bin/ruby ~/.mutt.tmpl.rb

  -r, --run=COMMAND             Run COMMAND and delete template afterwards.
  Instead of returning the path, tmpl runs COMMAND
  and deletes the mkstemp(3) file after COMMAND returns.
  It then exits with the return code of COMMAND.

  Example: tmpl -r "neomutt -F %f" ~/.mutt.tmpl.sh

  Variables:
          %f  - The generated mkstemp(3) file path

  -B, --background              Fork to background when used with -r
                                  (default=off)
      --stdout=FILE             Redirect STDOUT from -r to FILE
      --stderr=FILE             Redirect STDERR from -r to FILE

See tmpl(1) for more informations and examples.

```

## Manpage
```
TMPL(1)			    General Commands Manual		       TMPL(1)

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
